#ifndef SSVU_PREALLOCATOR
#define SSVU_PREALLOCATOR

#include <vector>
#include <stack>
#include <functional>
#include <stdexcept>
#include <cmath>
#include <cassert>
#include "SSVUtils/Global/Typedefs.h"
#include "SSVUtils/Utils/UtilsContainers.h"
#include "SSVUtils/MemoryManager/MemoryManager.h"

namespace ssvu
{
	namespace PreAlloc
	{
		using MemUnit = char;
		using MemUnitPtr = MemUnit*;
		using MemSize = decltype(sizeof(MemUnit)); // Should always be 1 byte

		template<typename T> inline constexpr MemSize getBytes(const T& mBytes) noexcept	{ return mBytes; }
		template<typename T> inline constexpr MemSize getKBsToBytes(const T& mKBs) noexcept	{ return mKBs * 1024; }
		template<typename T> inline constexpr MemSize getMBsToBytes(const T& mMBs) noexcept	{ return getKBsToBytes(mMBs * 1024); }
		template<typename T> inline constexpr MemSize getGBsToBytes(const T& mGBs) noexcept	{ return getMBsToBytes(mGBs * 1024); }

		namespace Internal
		{
			struct MemRange // MemRange is a range of memory [begin, end)
			{
				MemUnitPtr begin, end;
				inline MemRange(MemUnitPtr mStart, MemUnitPtr mEnd) noexcept : begin{mStart}, end{mEnd} { assert(mStart <= mEnd); }
				inline MemSize getSize() const noexcept { return sizeof(MemUnit) * (end - begin); }
			};

			class MemBuffer
			{
				private:
					Uptr<MemUnit[]> buffer;
					MemRange range;

				public:
					inline MemBuffer(MemSize mSize) : buffer{new MemUnit[mSize]}, range{&buffer[0], &buffer[mSize]} { assert(mSize > 0); }
					inline MemUnitPtr getBegin() const noexcept			{ return range.begin; }
					inline MemUnitPtr getEnd() const noexcept			{ return range.end; }
					inline const MemRange& getRange() const noexcept	{ return range; }
			};

			template<typename T> struct ContainerHelper;
			template<typename T, typename... TArgs> struct ContainerHelper<std::vector<T, TArgs...>>
			{
				template<typename... TItemArgs> inline static void emplace(std::vector<T, TArgs...>& mCont, TItemArgs&&... mArgs) { mCont.emplace_back(std::forward<TItemArgs>(mArgs)...); }
			};
			template<typename T, typename... TArgs> struct ContainerHelper<std::stack<T, TArgs...>>
			{
				template<typename... TItemArgs> inline static void emplace(std::stack<T, TArgs...>& mCont, TItemArgs&&... mArgs) { mCont.emplace(std::forward<TItemArgs>(mArgs)...); }
			};

			template<typename T, typename TCont> inline void destroyReclaim(T* mObj, TCont& mCont, MemSize mSize = sizeof(T))
			{
				auto objStart(reinterpret_cast<MemUnitPtr>(mObj));
				mObj->~T(); ContainerHelper<TCont>::emplace(mCont, objStart, objStart + mSize);
			}
			template<typename T> inline void createChunks(T& mContainer, MemSize mSize, std::size_t mChunks, const MemBuffer& mBuffer)
			{
				for(auto i(0u); i < mChunks; ++i)
				{
					MemUnitPtr chunkBegin(mBuffer.getBegin() + (i * mSize));
					ContainerHelper<T>::emplace(mContainer, chunkBegin, chunkBegin + mSize);
				}
			}

			template<typename T, typename TPreAlloc> struct PreAllocMMDel
			{
				TPreAlloc* preAlloc;
				PreAllocMMDel(TPreAlloc& mPreAlloc) : preAlloc(&mPreAlloc) { }
				inline void operator()(T* mPtr) { preAlloc->destroy(mPtr); }
			};

			template<typename T, typename TPreAlloc> class PreAllocMMBase : public ssvu::Internal::MemoryManagerBase<PreAllocMMBase<T, TPreAlloc>, T, PreAllocMMDel<T, TPreAlloc>>
			{
				private:
					using UptrDeleter = PreAllocMMDel<T, TPreAlloc>;
					TPreAlloc& preAlloc;
					UptrDeleter uptrDeleter;

				public:
					PreAllocMMBase(TPreAlloc& mPreAlloc) : preAlloc(mPreAlloc), uptrDeleter(preAlloc) { }

					inline void refreshImpl()
					{
						eraseRemoveIf(this->items, this->template isDead<Uptr<T, UptrDeleter>>);
						for(const auto& i : this->toAdd) this->items.emplace_back(i, uptrDeleter); this->toAdd.clear();
					}
					template<typename TType, typename... TArgs> inline TType& createTImpl(TArgs&&... mArgs)
					{
						auto result(this->preAlloc.template create<TType>(std::forward<TArgs>(mArgs)...)); this->toAdd.push_back(result); return *result;
					}
			};
		}

		class PreAllocDyn
		{
			private:
				using MemRange = Internal::MemRange;
				const Internal::MemBuffer buffer;
				std::vector<MemRange> available;

				inline void joinRangesAt(int mIdx)
				{
					if(mIdx + 1 != available.size() - 1) available[mIdx + 1] = available.back();
					available.pop_back();
				}

				inline void unifyContiguous()
				{
					std::size_t newNLast{0};

					do
					{
						int nLast{newNLast};
						newNLast = available.size() - 1;
						for(int i(available.size() - 2); i >= nLast; --i)
						{
							if(available[i].begin > available[i + 1].begin)
							{
								if(available[i].begin == available[i + 1].end)
								{
									available[i].begin = available[i + 1].begin;
									joinRangesAt(i);
								}
								else std::swap(available[i], available[i + 1]);
								newNLast = i + 1;
							}
							else if(available[i].end == available[i + 1].begin)
							{
								available[i].end = available[i + 1].end;
								joinRangesAt(i);
							}
						}
					} while(newNLast < available.size() - 1);
				}

			public:
				PreAllocDyn(MemSize mBufferSize) : buffer{mBufferSize} { available.push_back(buffer.getRange()); }
				template<typename T, typename... TArgs> inline T* create(TArgs&&... mArgs)
				{
					const auto& reqSize(sizeof(T));
					for(auto itr(std::begin(available)); itr != std::end(available); ++itr)
					{
						if(itr->getSize() < reqSize) continue;
						MemUnitPtr toUse{itr->begin};
						itr->begin = toUse + reqSize;
						if(itr->getSize() == 0) available.erase(itr);
						return new (toUse) T{std::forward<TArgs>(mArgs)...};
					}

					throw std::runtime_error{"Dynamic preallocator could not allocate object"};
				}
				template<typename T> inline void destroy(T* mObj, MemSize mSize = sizeof(T)) { Internal::destroyReclaim(mObj, available, mSize); unifyContiguous(); }
		};

		class PreAllocChunk
		{
			protected:
				using MemRange = Internal::MemRange;
				const MemSize chunkSize;
				const Internal::MemBuffer buffer;
				std::stack<MemRange, std::vector<MemRange>> available;

			public:
				PreAllocChunk(MemSize mChunkSize, std::size_t mChunks) : chunkSize{mChunkSize}, buffer{chunkSize * mChunks} { assert(mChunks > 0); createChunks(available, mChunkSize, mChunks, buffer); }
				template<typename T, typename... TArgs> inline T* create(TArgs&&... mArgs)
				{
					assert(sizeof(T) <= chunkSize);
					auto toUse(available.top().begin); available.pop();
					return new (toUse) T{std::forward<TArgs>(mArgs)...};
				}
				template<typename T> inline void destroy(T* mObj) { Internal::destroyReclaim(mObj, available, chunkSize); }
		};

		template<typename T> class PreAllocStatic
		{
			protected:
				using MemRange = Internal::MemRange;
				const Internal::MemBuffer buffer;
				std::stack<MemRange, std::vector<MemRange>> available;

			public:
				PreAllocStatic(std::size_t mChunks) : buffer{sizeof(T) * mChunks} { assert(mChunks > 0); createChunks(available, sizeof(T), mChunks, buffer); }
				template<typename TType = T, typename... TArgs> inline T* create(TArgs&&... mArgs)
				{
					auto toUse(available.top().begin); available.pop();
					return new (toUse) T{std::forward<TArgs>(mArgs)...};
				}
				inline void destroy(T* mObj) { Internal::destroyReclaim(mObj, available); }
		};

		template<typename T> using PAMMDyn =	Internal::PreAllocMMBase<T, PreAllocDyn>;
		template<typename T> using PAMMChunk =	Internal::PreAllocMMBase<T, PreAllocChunk>;
		template<typename T> using PAMMStatic =	Internal::PreAllocMMBase<T, PreAllocStatic<T>>;
	}
}

#endif
