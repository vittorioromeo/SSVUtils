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
				inline bool isEmpty() const noexcept { return ptrs.empty(); }
		};

		template<typename T, typename TBase> inline PtrRecycler<T, TBase>& getPtrRecycler() noexcept
		{
			thread_local PtrRecycler<T, TBase> result;
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

	template<typename T, typename TBase> using UptrRecPoly = Uptr<T, void(*)(TBase*)>;
	template<typename T> using UptrRec = UptrRecPoly<T, T>;

	template<typename T, typename TBase, typename... TArgs> inline UptrRecPoly<T, TBase> makeUptrRecPoly(TArgs&&... mArgs)
	{
		return UptrRecPoly<T, TBase>(Internal::makePolyPtr<T, TBase>(std::forward<TArgs>(mArgs)...), [](TBase* mPtr)
		{
			auto& pr(Internal::getPtrRecycler<T, TBase>());
			pr.destroy(mPtr); pr.push(mPtr);
		});
	}
	template<typename T, typename... TArgs> inline UptrRec<T> makeUptrRec(TArgs&&... mArgs) { return makeUptrRecPoly<T, T>(std::forward<TArgs>(mArgs)...); }

	template<typename T, typename TBase, typename... TArgs, typename TC> inline T& getEmplaceUptrRecPoly(TC& mContainer, TArgs&&... mArgs)
	{
		return Internal::getEmplaceUptrImpl<T, TC, decltype(&makeUptrRecPoly<T, TBase, TArgs...>), &makeUptrRecPoly<T, TBase, TArgs...>>(mContainer, std::forward<TArgs>(mArgs)...);
	}
	template<typename T, typename... TArgs, typename TC> inline T& getEmplaceUptrRec(TC& mContainer, TArgs&&... mArgs)
	{
		return getEmplaceUptrRecPoly<T, T>(mContainer, std::forward<TArgs>(mArgs)...);
	}

	template<typename TBase> using VecUptrRecPoly = std::vector<UptrRec<TBase>>;
}

#endif

// TODO: docs, tests
