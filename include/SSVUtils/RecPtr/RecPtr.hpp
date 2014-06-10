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
					SSVU_ASSERT_STATIC(isBaseOf<TT, T>() || isSame<TT, T>(), "TT must be the same as T or its base type");

					alloc.destroy(reinterpret_cast<T*>(mPtr));
					ptrs.emplace_back(reinterpret_cast<T*>(mPtr));
				}

				template<typename... TArgs> inline T* create(TArgs&&... mArgs)
				{
					auto result(ptrs.empty() ? alloc.allocate(1) : pop());
					alloc.construct(result, std::forward<TArgs>(mArgs)...);
					return result;
				}
		};

		template<typename T> inline Recycler<T>& getRecycler() noexcept
		{
			static Recycler<T> result; return result;
		}
	}

	template<typename T, typename TBase> using UptrRecPoly = Uptr<T, void(*)(TBase*)>;
	template<typename T> using UptrRec = UptrRecPoly<T, T>;
	template<typename TBase> using VecUptrRec = std::vector<UptrRec<TBase>>;

	template<typename T, typename TBase, typename... TArgs> inline UptrRecPoly<T, TBase> makeUptrRecPoly(TArgs&&... mArgs)
	{
		return {Internal::getRecycler<T>().create(std::forward<TArgs>(mArgs)...), [](TBase* mPtr){ Internal::getRecycler<T>().recycle(mPtr); }};
	}
	template<typename T, typename... TArgs> inline UptrRec<T> makeUptrRec(TArgs&&... mArgs) { return makeUptrRecPoly<T, T>(std::forward<TArgs>(mArgs)...); }

	namespace Internal
	{
		template<typename T, typename TBase> struct MakerUptrRecPoly
		{
			template<typename... TArgs> static inline UptrRecPoly<T, TBase> make(TArgs&&... mArgs)
			{
				return makeUptrRecPoly<T, TBase, TArgs...>(std::forward<TArgs>(mArgs)...);
			}
		};
	}

	template<typename T, typename TBase, typename... TArgs, typename TC> inline T& getEmplaceUptrRecPoly(TC& mContainer, TArgs&&... mArgs)
	{
		return Internal::getEmplaceUptrImpl<T, TC, Internal::MakerUptrRecPoly<T, TBase>>(mContainer, std::forward<TArgs>(mArgs)...);
	}
	template<typename T, typename... TArgs, typename TC> inline T& getEmplaceUptrRec(TC& mContainer, TArgs&&... mArgs)
	{
		return getEmplaceUptrRecPoly<T, T>(mContainer, std::forward<TArgs>(mArgs)...);
	}
}

#endif

// TODO: docs, test, sptr
