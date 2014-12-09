// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_CORE_UTILS_TUPLE
#define SSVU_CORE_UTILS_TUPLE

namespace ssvu
{
	namespace Internal
	{
		template<SizeT N> struct Exploder
		{
			template<typename TF, typename T, typename... TArgs> inline static auto explode(TF&& mF, T&& mT, TArgs&&... mArgs) noexcept
			{
				return Exploder<N - 1>::explode(fwd<TF>(mF), fwd<T>(mT), ::std::get<N - 1>(fwd<T>(mT)), fwd<TArgs>(mArgs)...);
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

		template<typename... TArgs> struct ForHelper
		{
			template<SizeT TI, typename T, typename TF> inline static EnableIf<TI == sizeof...(TArgs), void> exec(T&&, TF) noexcept { }

			template<SizeT TI = 0, typename T, typename TF> inline static EnableIf<TI < sizeof...(TArgs), void> exec(T&& mT, TF&& mF)
				noexcept(true)
			{
				fwd<TF>(mF)(std::get<TI>(fwd<T>(mT)));
				exec<TI + 1, T, TF>(fwd<T>(mT), fwd<TF>(mF));
			}
		};

		template<typename... TArgs> struct ForIdxHelper
		{
			template<SizeT TI, typename T, typename TF> inline static EnableIf<TI == sizeof...(TArgs), void> exec(T&&, TF) noexcept { }

			template<SizeT TI = 0, typename T, typename TF> inline static EnableIf<TI < sizeof...(TArgs), void> exec(T&& mT, TF&& mF)
				noexcept(true)
			{
				fwd<TF>(mF)(TI, std::get<TI>(fwd<T>(mT)));
				exec<TI + 1, T, TF>(fwd<T>(mT), fwd<TF>(mF));
			}
		};

		template<typename T, SizeT TI> using IndexDep = T;
		template<typename T, SizeT TN, typename TIdxs = IdxSeq<TN>> struct TplRepeatImpl;

		template<typename T, SizeT TN, SizeT... TIdxs>
		struct TplRepeatImpl<T, TN, IdxSeq<TIdxs...>>
		{
			using Type = std::tuple<IndexDep<T, TIdxs>...>;
		};
	}

	template<typename T, SizeT TN, typename TIdxs = IdxSeq<TN>>
	using TplRepeat = typename Internal::TplRepeatImpl<T, TN, TIdxs>::Type;

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

	#define SSVU_DEFINE_TPLFOR(mName, mType, mHelper) \
		template<typename TF, typename... TArgs> inline void mName (mType mT, TF&& mF) \
			noexcept(noexcept(Internal:: mHelper <TArgs...>::exec(mT, fwd<TF>(mF)))) \
		{ \
			Internal:: mHelper <TArgs...>::exec(mT, fwd<TF>(mF)); \
		}

	SSVU_DEFINE_TPLFOR(tplFor, std::tuple<TArgs...>&, ForHelper)
	SSVU_DEFINE_TPLFOR(tplFor, const std::tuple<TArgs...>&, ForHelper)
	SSVU_DEFINE_TPLFOR(tplFor, std::tuple<TArgs...>&&, ForHelper)

	SSVU_DEFINE_TPLFOR(tplFor, std::pair<TArgs...>&, ForHelper)
	SSVU_DEFINE_TPLFOR(tplFor, const std::pair<TArgs...>&, ForHelper)
	SSVU_DEFINE_TPLFOR(tplFor, std::pair<TArgs...>&&, ForHelper)

	SSVU_DEFINE_TPLFOR(tplForIdx, std::tuple<TArgs...>&, ForIdxHelper)
	SSVU_DEFINE_TPLFOR(tplForIdx, const std::tuple<TArgs...>&, ForIdxHelper)
	SSVU_DEFINE_TPLFOR(tplForIdx, std::tuple<TArgs...>&&, ForIdxHelper)

	SSVU_DEFINE_TPLFOR(tplForIdx, std::pair<TArgs...>&, ForIdxHelper)
	SSVU_DEFINE_TPLFOR(tplForIdx, const std::pair<TArgs...>&, ForIdxHelper)
	SSVU_DEFINE_TPLFOR(tplForIdx, std::pair<TArgs...>&&, ForIdxHelper)

	#undef SSVU_DEFINE_TPLFOR
}

#endif

// TODO: docs

