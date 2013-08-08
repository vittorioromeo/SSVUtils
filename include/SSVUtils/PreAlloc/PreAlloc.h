#ifndef SSVU_PREALLOCATOR
#define SSVU_PREALLOCATOR

#include <vector>
#include <stack>
#include <algorithm>
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

		template<typename T> constexpr MemSize getKBsToBytes(const T& mValue) { return mValue * 1024; }
		template<typename T> constexpr MemSize getMBsToBytes(const T& mValue) { return mValue * 1024 * 1024; }
		template<typename T> constexpr MemSize getGBsToBytes(const T& mValue) { return mValue * 1024 * 1024 * 1024; }

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
		}

		class PreAllocator
		{
			private:
				using MemRange = Internal::MemRange;
				Internal::MemBuffer buffer;
				std::vector<MemRange> available;

				inline void unifyFrom(unsigned int mIndex)
				{
					MemUnitPtr lastEnd(available[mIndex].end);
					auto toChange(std::begin(available) + mIndex);
					auto itr(toChange + 1);

					for(; itr != std::end(available); ++itr)
						if(itr->begin == lastEnd) lastEnd = itr->end;
						else break;

					// Erase all but the first unified elements, then change the first one with
					// the updated range

					available.erase(toChange + 1, itr);
					toChange->begin = available[mIndex].begin;
					toChange->end = lastEnd;
				}

				inline void unifyContiguous()
				{
					sort(available, [](const MemRange& mA, const MemRange& mB){ return mA.begin < mB.begin; });
					//for(auto i(0u); i < available.size(); ++i) unifyFrom(i);
					unifyFrom(0);
				}

				inline std::vector<MemRange>::iterator findSuitableMemory(MemSize mRequiredSize)
				{
					// Tries to find a memory piece big enough to hold mRequiredSize
					// If it is not found, contiguous memory pieces are unified
					// If it is not found again, throws an exception

					for(int i{0}; i < 2; ++i)
					{
						for(auto itr(std::begin(available)); itr != std::end(available); ++itr) if(itr->getSize() >= mRequiredSize) return itr;
						unifyContiguous();
					}
					throw std::runtime_error("PreAllocator couldn't find suitable memory (required: " + toStr(mRequiredSize) + ")");
				}

			public:
				PreAllocator(MemSize mBufferSize) : buffer{mBufferSize}
				{
					// Add the whole buffer to the available memory vector
					available.push_back(buffer.getRange()); return;
				}

				template<typename T, typename... TArgs> inline T* create(TArgs&&... mArgs)
				{
					// Creates and returns a T* allocated with "placement new" on an available piece of the buffer
					// T must be the "real object type" - this method will fail with pointers to bases that store derived instances!

					const auto& requiredSize(sizeof(T));
					const auto& suitable(findSuitableMemory(requiredSize));

					MemUnitPtr toUse{suitable->begin};
					MemRange leftover{toUse + requiredSize, suitable->end};

					available.erase(suitable);
					if(leftover.getSize() > 0) available.push_back(leftover);

					return new (toUse) T{std::forward<TArgs>(mArgs)...};
				}
				template<typename T> inline void destroy(T* mObject)
				{
					// Destroys a previously allocated object, calling its destructor and reclaiming its memory piece
					// T must be the "real object type" - this method will fail with pointers to bases that store derived instances!

					auto objStart(reinterpret_cast<MemUnitPtr>(mObject));
					available.emplace_back(objStart, objStart + sizeof(T));
					mObject->~T();
				}
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
				template<typename T> inline void destroy(T* mObject)
				{
					// Destroys a previously allocated object, calling its destructor and reclaiming its memory piece
					// T must be the "real object type" - this method will fail with pointers to bases that store derived instances!

					auto objStart(reinterpret_cast<MemUnitPtr>(mObject));
					available.emplace(objStart, objStart + chunkSize);
					mObject->~T();
				}
		};

		template<typename T> class PreAllocatorStatic : public PreAllocatorChunk
		{
			public:
				PreAllocatorStatic(unsigned int mChunks) : PreAllocatorChunk{sizeof(T), mChunks} { }
				template<typename... TArgs> inline T* create(TArgs&&... mArgs) { return PreAllocatorChunk::create<T, TArgs...>(std::forward<TArgs>(mArgs)...); }
				inline void destroy(T* mObject) { PreAllocatorChunk::destroy<T>(mObject); }
		};

		template<typename T> class PreAllocMMStatic : public ssvu::Internal::MemoryManagerBase<PreAllocMMStatic<T>, T, std::function<void(T*)>>
		{
			private:
				using UptrDeleter = std::function<void(T*)>;

			private:
				PreAllocatorStatic<T> preAllocator{20000};
				UptrDeleter uptrDeleter;

			public:
				PreAllocMMStatic() : uptrDeleter{[&](T* mPtr){ preAllocator.destroy(mPtr); }} { }

				inline void refreshImpl()
				{
					eraseRemoveIf(this->items, this->template isDead<Uptr<T, UptrDeleter>>);
					for(const auto& i : this->toAdd) this->items.emplace_back(i, uptrDeleter); this->toAdd.clear();
				}
				template<typename TType, typename... TArgs> inline TType& createTImpl(TArgs&&...)
				{
					throw std::runtime_error("this should not be called!");
				}
				template<typename... TArgs> inline T& createImpl(TArgs&&... mArgs)
				{
					auto result(preAllocator.create(std::forward<TArgs>(mArgs)...)); this->toAdd.push_back(result); return *result;
				}
		};
	}
}

#endif
