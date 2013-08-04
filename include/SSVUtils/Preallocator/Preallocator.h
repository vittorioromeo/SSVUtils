#ifndef SSVU_PREALLOCATOR
#define SSVU_PREALLOCATOR

#include <vector>
#include "SSVUtils/Global/Typedefs.h"

namespace ssvu
{
	class PreAllocator
	{
		private:
			using MemoryUnit = char;
			using MemoryPtr = MemoryUnit*;
			using MemorySize = decltype(sizeof(MemoryUnit));

			struct Piece
			{
				// Piece is a range of memory [begin, end)
				MemoryPtr begin, end;
				inline Piece(MemoryPtr mStart, MemoryPtr mEnd) : begin{mStart}, end{mEnd} { }
				inline MemorySize getSize() const { return sizeof(MemoryUnit) * (end - begin); }
			};

			Uptr<MemoryUnit[]> buffer;
			std::vector<Piece> available;

			inline void unifyFrom(unsigned int mIndex)
			{
				MemoryPtr lastEnd(available[mIndex].end);
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
				std::sort(std::begin(available), std::end(available), [](const Piece& mA, const Piece& mB){ return mA.begin < mB.begin; });
				for(unsigned int i{0}; i < available.size(); ++i) unifyFrom(i);
			}

			inline std::vector<Piece>::iterator findSuitableMemory(MemorySize mRequiredSize)
			{
				// Tries to find a memory piece big enough to hold mRequiredSize
				// If it is not found, contiguous memory pieces are unified
				// If it is not found again, throws an exception

				for(int i{0}; i < 2; ++i)
				{
					for(auto itr(std::begin(available)); itr != std::end(available); ++itr) if(itr->getSize() >= mRequiredSize) return itr;
					unifyContiguous();
				}
				throw;
			}

		public:
			PreAllocator(MemorySize mBufferSize) : buffer{new MemoryUnit[mBufferSize]}
			{
				// Add the whole buffer to the available memory vector
				available.emplace_back(&buffer[0], &buffer[mBufferSize]);
			}

			template<typename T, typename... TArgs> inline T* create(TArgs&&... mArgs)
			{
				// Creates and returns a T* allocated with "placement new" on an available piece of the buffer
				// T must be the "real object type" - this method will fail with pointers to bases that store derived instances!

				const auto& requiredSize(sizeof(T));
				const auto& suitable(findSuitableMemory(requiredSize));

				MemoryPtr toUse{suitable->begin};
				Piece leftover{toUse + requiredSize, suitable->end};

				available.erase(suitable);
				if(leftover.getSize() > 0) available.push_back(leftover);

				return new (toUse) T{std::forward<TArgs>(mArgs)...};
			}
			template<typename T> inline void destroy(T* mObject)
			{
				// Destroys a previously allocated object, calling its destructor and reclaiming its memory piece
				// T must be the "real object type" - this method will fail with pointers to bases that store derived instances!

				mObject->~T();
				auto objStart(reinterpret_cast<MemoryPtr>(mObject));
				available.emplace_back(objStart, objStart + sizeof(T));
			}
	};
}

#endif
