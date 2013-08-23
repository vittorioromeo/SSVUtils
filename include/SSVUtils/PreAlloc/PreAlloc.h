#ifndef SSVU_PREALLOCATOR
#define SSVU_PREALLOCATOR

#include <vector>
#include <stack>
#include <functional>
#include <stdexcept>
#include <cmath>
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

		template<typename T> inline constexpr MemSize getBytes(const T& mBytes)		{ return mBytes; }
		template<typename T> inline constexpr MemSize getKBsToBytes(const T& mKBs)	{ return mKBs * 1024; }
		template<typename T> inline constexpr MemSize getMBsToBytes(const T& mMBs)	{ return getKBsToBytes(mMBs * 1024); }
		template<typename T> inline constexpr MemSize getGBsToBytes(const T& mGBs)	{ return getMBsToBytes(mGBs * 1024); }

		namespace Internal
		{
			struct MemRange
			{
				// MemRange is a range of memory [begin, end)
				MemUnitPtr begin, end;
				inline MemRange(MemUnitPtr mStart, MemUnitPtr mEnd) : begin{mStart}, end{mEnd} { }
				inline MemSize getSize() const { return sizeof(MemUnit) * (end - begin); }
			};

			class MemBuffer
			{
				private:
					Uptr<MemUnit[]> buffer;
					MemRange range;

				public:
					inline MemBuffer(MemSize mSize) : buffer{new MemUnit[mSize]}, range{&buffer[0], &buffer[mSize]} { }
					inline MemUnitPtr getBegin() const		{ return range.begin; }
					inline MemUnitPtr getEnd() const		{ return range.end; }
					inline const MemRange& getRange() const	{ return range; }
			};

			template<typename T> inline MemRange destroyAndGetMemRange(T* mObject, MemSize mSize)
			{
				// Destroys a previously allocated object, calling its destructor and reclaiming its memory piece
				// T must be the "real object type" - this method will fail with pointers to bases that store derived instances!

				auto objStart(reinterpret_cast<MemUnitPtr>(mObject));
				mObject->~T(); return {objStart, objStart + mSize};
			}

			template<typename T, typename TPreAlloc> class PreAllocMMBase : public ssvu::Internal::MemoryManagerBase<PreAllocMMBase<T, TPreAlloc>, T, std::function<void(T*)>>
			{
				private:
					using UptrDeleter = std::function<void(T*)>;

				private:
					TPreAlloc& preAllocator;
					UptrDeleter uptrDeleter;

				public:
					PreAllocMMBase(TPreAlloc& mPreAllocator) : preAllocator(mPreAllocator), uptrDeleter{[&](T* mPtr){ preAllocator.destroy(mPtr); }} { }

					inline void refreshImpl()
					{
						eraseRemoveIf(this->items, this->template isDead<Uptr<T, UptrDeleter>>);
						for(const auto& i : this->toAdd) this->items.emplace_back(i, uptrDeleter); this->toAdd.clear();
					}
					template<typename TType, typename... TArgs> inline TType& createTImpl(TArgs&&... mArgs)
					{
						auto result(this->preAllocator.template create<TType>(std::forward<TArgs>(mArgs)...)); this->toAdd.push_back(result); return *result;
					}
			};
		}

		class PreAllocatorDynamic
		{
			private:
				using MemRange = Internal::MemRange;
				Internal::MemBuffer buffer;
				std::vector<MemRange> available;

				inline void unifyContiguous()
				{
					sort(available, [](const MemRange& mA, const MemRange& mB){ return mA.begin < mB.begin; });
					for(auto itr(std::begin(available)); itr != std::end(available) - 1;)
					{
						auto& next(*(itr + 1));
						if(itr->end != next.begin) { ++itr; continue; }

						next.begin = itr->begin;
						available.erase(itr);
					}
				}

			public:
				PreAllocatorDynamic(MemSize mBufferSize) : buffer{mBufferSize} { available.push_back(buffer.getRange()); }

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
				}
				template<typename T> inline void destroy(T* mObject, MemSize mSize)
				{
					available.push_back(Internal::destroyAndGetMemRange<T>(mObject, mSize));
					unifyContiguous();
				}
				template<typename T> inline void destroy(T* mObject) { destroy(mObject, sizeof(T)); }
		};

		class PreAllocatorChunk
		{
			protected:
				using MemRange = Internal::MemRange;
				MemSize chunkSize;
				Internal::MemBuffer buffer;
				std::stack<MemRange, std::vector<MemRange>> available;

			public:
				PreAllocatorChunk(MemSize mChunkSize, unsigned int mChunks) : chunkSize{mChunkSize}, buffer{chunkSize * mChunks}
				{
					for(auto i(0u); i < mChunks; ++i)
					{
						MemUnitPtr chunkBegin(buffer.getBegin() + (i * chunkSize));
						available.emplace(chunkBegin, chunkBegin + chunkSize);
					}
				}

				template<typename T, typename... TArgs> inline T* create(TArgs&&... mArgs)
				{
					auto toUse(available.top().begin); available.pop();
					return new (toUse) T{std::forward<TArgs>(mArgs)...};
				}
				template<typename T> inline void destroy(T* mObject) { available.emplace(Internal::destroyAndGetMemRange<T>(mObject, chunkSize)); }
		};

		template<typename T> struct PreAllocatorStatic : public PreAllocatorChunk
		{
			PreAllocatorStatic(unsigned int mChunks) : PreAllocatorChunk{sizeof(T), mChunks} { }
			template<typename TType = T, typename... TArgs> inline T* create(TArgs&&... mArgs) { return PreAllocatorChunk::create<TType, TArgs...>(std::forward<TArgs>(mArgs)...); }
		};

		template<typename T> using PAMMDynamic =	Internal::PreAllocMMBase<T, PreAllocatorDynamic>;
		template<typename T> using PAMMChunk =		Internal::PreAllocMMBase<T, PreAllocatorChunk>;
		template<typename T> using PAMMStatic =		Internal::PreAllocMMBase<T, PreAllocatorStatic<T>>;
	}
}

#endif
