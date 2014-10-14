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
		template<typename T> class Recycler
		{
			private:
				std::vector<T*> ptrs;
				std::allocator<T> alloc;

				inline T* pop() noexcept
				{
					SSVU_ASSERT(!ptrs.empty());

					auto result(ptrs.back());
					ptrs.pop_back();
					return reinterpret_cast<T*>(result);
				}

			public:
				inline Recycler() = default;
				inline ~Recycler() noexcept { for(auto p : ptrs) alloc.deallocate(reinterpret_cast<T*>(p), 1); }

				template<typename TT> inline void recycle(TT* mPtr) noexcept(noexcept(alloc.destroy(mPtr)) && noexcept(ptrs.emplace_back(mPtr)))
				{
					SSVU_ASSERT(mPtr != nullptr);
					SSVU_ASSERT_STATIC(isSameOrBaseOf<TT, T>(), "TT and T must belong to the same hierarchy");

					alloc.destroy(reinterpret_cast<T*>(mPtr));
					ptrs.emplace_back(reinterpret_cast<T*>(mPtr));
				}

				template<typename... TArgs> inline T* create(TArgs&&... mArgs)
				{
					auto result(ptrs.empty() ? alloc.allocate(1) : pop());
					alloc.construct(result, fwd<TArgs>(mArgs)...);
					return result;
				}
		};

		template<typename T> inline auto& getRecycler() noexcept { thread_local Recycler<T> result; return result; }
	}

	template<typename T, typename TBase> using UPtrRecPoly = UPtr<T, void(*)(TBase*)>;
	template<typename T> using UPtrRec = UPtrRecPoly<T, T>;
	template<typename TBase> using VecUPtrRec = std::vector<UPtrRec<TBase>>;

	template<typename T, typename TBase, typename... TArgs> inline UPtrRecPoly<T, TBase> makeUPtrRecPoly(TArgs&&... mArgs)
	{
		return {Internal::getRecycler<T>().create(fwd<TArgs>(mArgs)...), [](TBase* mPtr){ Internal::getRecycler<T>().recycle(mPtr); }};
	}
	template<typename T, typename... TArgs> inline UPtrRec<T> makeUPtrRec(TArgs&&... mArgs) { return makeUPtrRecPoly<T, T>(fwd<TArgs>(mArgs)...); }

	namespace Internal
	{
		template<typename T, typename TBase> struct MakerUPtrRecPoly
		{
			template<typename... TArgs> inline static UPtrRecPoly<T, TBase> make(TArgs&&... mArgs)
			{
				return makeUPtrRecPoly<T, TBase, TArgs...>(fwd<TArgs>(mArgs)...);
			}
		};
	}

	template<typename T, typename TBase, typename... TArgs, typename TC> inline T& getEmplaceUPtrRecPoly(TC& mContainer, TArgs&&... mArgs)
	{
		return Internal::getEmplaceUPtrImpl<T, TC, Internal::MakerUPtrRecPoly<T, TBase>>(mContainer, fwd<TArgs>(mArgs)...);
	}
	template<typename T, typename... TArgs, typename TC> inline T& getEmplaceUPtrRec(TC& mContainer, TArgs&&... mArgs)
	{
		return getEmplaceUPtrRecPoly<T, T>(mContainer, fwd<TArgs>(mArgs)...);
	}
}

#endif

// TODO: docs, test, sPtr
