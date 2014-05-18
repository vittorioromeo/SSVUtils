// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_RECPTR
#define SSVU_RECPTR

#include "SSVUtils/Core/Core.hpp"

namespace ssvu
{
	namespace Internal
	{
		template<typename T, typename TBase> class PtrRecycler
		{
			private:
				std::vector<TBase*> ptrs;
				std::allocator<T> alloc;

			public:
				inline PtrRecycler() = default;
				inline ~PtrRecycler() noexcept { for(auto p : ptrs) alloc.deallocate(reinterpret_cast<T*>(p), 1); }

				inline T* allocate() noexcept(noexcept(alloc.allocate(1))) { return alloc.allocate(1); }

				template<typename... TArgs> inline void construct(T* mPtr, TArgs&&... mArgs)
					noexcept(noexcept(alloc.construct(mPtr, std::forward<TArgs>(mArgs)...)))
				{
					alloc.construct(mPtr, std::forward<TArgs>(mArgs)...);
				}

				inline void destroy(TBase* mPtr) noexcept(noexcept(alloc.destroy(mPtr)))
				{
					alloc.destroy(mPtr);
				}

				inline void push(TBase* mPtr) noexcept
				{
					SSVU_ASSERT(mPtr != nullptr);

					ptrs.emplace_back(mPtr);
				}
				inline T* pop() noexcept
				{
					SSVU_ASSERT(ptrs.size() > 0);

					auto result(ptrs.back());
					ptrs.pop_back();
					return reinterpret_cast<T*>(result);
				}
				inline bool isEmpty() const noexcept { return ptrs.size() == 0; }
		};

		template<typename T, typename TBase> inline PtrRecycler<T, TBase>& getPtrRecycler() noexcept
		{
			static PtrRecycler<T, TBase> result;
			return result;
		}

		template<typename T, typename TBase, typename... TArgs> inline T* makePolyPtr(TArgs&&... mArgs)
		{
			auto& pr(getPtrRecycler<T, TBase>());
			T* result{pr.isEmpty() ? pr.allocate() : pr.pop()};
			pr.construct(result, std::forward<TArgs>(mArgs)...);
			return result;
		}
	}

	template<typename T, typename TBase> using RecPolyUptr = Uptr<T, void(*)(TBase*)>;
	template<typename T> using RecUptr = RecPolyUptr<T, T>;

	template<typename T, typename TBase, typename... TArgs> inline RecPolyUptr<T, TBase> makeRecPolyUptr(TArgs&&... mArgs)
	{
		return RecPolyUptr<T, TBase>(Internal::makePolyPtr<T, TBase>(std::forward<TArgs>(mArgs)...), [](TBase* mPtr)
		{
			auto& pr(Internal::getPtrRecycler<T, TBase>());
			pr.destroy(mPtr); pr.push(mPtr);
		});
	}
	template<typename T, typename... TArgs> inline RecUptr<T> makeRecUptr(TArgs&&... mArgs) { return makeRecPolyUptr<T, T>(std::forward<TArgs>(mArgs)...); }

	template<typename T, typename TBase, typename... TArgs, typename TC> inline T& getEmplaceRecPolyUptr(TC& mContainer, TArgs&&... mArgs)
	{
		auto uptr(makeRecPolyUptr<T, TBase>(std::forward<TArgs>(mArgs)...));
		auto result(uptr.get());
		mContainer.emplace_back(std::move(uptr));
		return *result;
	}
	template<typename T, typename... TArgs, typename TC> inline T& getEmplaceRecUptr(TC& mContainer, TArgs&&... mArgs)
	{
		return getEmplaceRecPolyUptr<T, T>(mContainer, std::forward<TArgs>(mArgs)...);
	}

	template<typename TBase> using RecPolyUptrVector = std::vector<RecUptr<TBase>>;
}

#endif

// TODO: docs, tests
