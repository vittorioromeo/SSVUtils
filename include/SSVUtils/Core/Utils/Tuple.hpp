// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_CORE_UTILS_TUPLE
#define SSVU_CORE_UTILS_TUPLE

#include "SSVUtils/UnionVariant/Internal/CTUtils.hpp"

namespace ssvu
{
	namespace Internal
	{
		template<SizeT TN> struct Exploder
		{
			template<typename TF, typename T, typename... TArgs> inline static auto explode(TF&& mF, T&& mT, TArgs&&... mArgs) noexcept
			{
				return Exploder<TN - 1>::explode(fwd<TF>(mF), fwd<T>(mT), ::std::get<TN - 1>(fwd<T>(mT)), fwd<TArgs>(mArgs)...);
			}
		};

		template<> struct Exploder<0>
		{
			template<typename TF, typename T, typename... TArgs> inline static auto explode(TF&& mF, T&&, TArgs&&... mArgs)
				noexcept(noexcept(fwd<TF>(mF)(fwd<TArgs>(mArgs)...)))
			{
				return fwd<TF>(mF)(fwd<TArgs>(mArgs)...);
			}
		};

		template<SizeT TS, typename... TTpls> struct ForHelper
		{
			template<SizeT TI, typename TF> inline static EnableIf<TI == TS, void> exec(TF, TTpls&&...) noexcept { }

			template<SizeT TI = 0, typename TF> inline static EnableIf<TI < TS, void> exec(TF&& mF, TTpls&&... mTpls)
				noexcept(true)
			{
				fwd<TF>(mF)(std::get<TI>(fwd<TTpls>(mTpls))...);
				exec<TI + 1, TF>(fwd<TF>(mF), fwd<TTpls>(mTpls)...);
			}
		};

		template<SizeT TS, typename... TTpls> struct ForIdxHelper
		{
			template<SizeT TI, typename TF> inline static EnableIf<TI == TS, void> exec(TF, TTpls&&...) noexcept { }

			template<SizeT TI = 0, typename TF> inline static EnableIf<TI < TS, void> exec(TF&& mF, TTpls&&... mTpls)
				noexcept(true)
			{
				fwd<TF>(mF)(TI, std::get<TI>(fwd<TTpls>(mTpls))...);
				exec<TI + 1, TF>(fwd<TF>(mF), fwd<TTpls>(mTpls)...);
			}
		};

		template<typename, SizeT, typename> struct TplRepeatImplHelper;

		template<typename T, SizeT TS, typename... TArgs> struct TplRepeatImplHelper<T, TS, std::tuple<TArgs...>>
		{
			using Type = typename TplRepeatImplHelper<T, TS - 1, std::tuple<TArgs..., T>>::Type;
		};

		template<typename T, typename... TArgs> struct TplRepeatImplHelper<T, 0, std::tuple<TArgs...>>
		{
			using Type = std::tuple<TArgs...>;
		};

		template<typename T, SizeT TS> struct TplRepeatImpl
		{
			using Type = typename TplRepeatImplHelper<T, TS, std::tuple<>>::Type;
		};
	}

	template<typename T, SizeT TN, typename TIdxs = IdxSeq<TN>>
	using TplRepeat = typename Internal::TplRepeatImpl<T, TN>::Type;

	template<typename T, typename TTpl> struct TplIdxOf;

	template<typename T, typename... TTypes>
	struct TplIdxOf<T, std::tuple<T, TTypes...>>
	{
		static constexpr SizeT value{0u};
	};

	template<typename T1, typename T2, typename... TTypes>
	struct TplIdxOf<T1, std::tuple<T2, TTypes...>>
	{
		static constexpr SizeT value{1 + TplIdxOf<T1, std::tuple<TTypes...>>::value};
	};

	template<typename TF, typename T> inline auto explode(TF&& mF, T&& mT)
		noexcept(noexcept(Internal::Exploder<getTplSize<Decay<T>>()>::explode(fwd<TF>(mF), fwd<T>(mT))))
	{
		return Internal::Exploder<getTplSize<Decay<T>>()>::explode(fwd<TF>(mF), fwd<T>(mT));
	}

	template<typename TF, typename... TTpls> inline void tplFor(TF&& mF, TTpls&&... mTpls)
		noexcept(true)
	{
		// TODO: change to static_assert that sizes are the same or min size
		constexpr SizeT maxSize(Internal::CTMax<getTplSize<TTpls>()...>::value);
		Internal::ForHelper<maxSize, TTpls...>::exec(fwd<TF>(mF), fwd<TTpls>(mTpls)...);
	}

	template<typename TF, typename... TTpls> inline void tplForIdx(TF&& mF, TTpls&&... mTpls)
		noexcept(true)
	{
		// TODO: change to static_assert that sizes are the same or min size
		constexpr SizeT maxSize(Internal::CTMax<getTplSize<TTpls>()...>::value);
		Internal::ForIdxHelper<maxSize, TTpls...>::exec(fwd<TF>(mF), fwd<TTpls>(mTpls)...);
	}
}

#endif

// TODO: docs, fix, noexcept(...), static asserts for tuple sizes
// TODO: static assert that:
//	1) arity of the passed function is equal to tuple count
//	2) tuple type count is equal (or loop until min value?)
