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
	}

	template<typename TF, typename T> inline auto explode(TF&& mF, T&& mT)
		noexcept(noexcept(Internal::Exploder<getTplSize<Decay<T>>()>::explode(fwd<TF>(mF), fwd<T>(mT))))
	{
		return Internal::Exploder<getTplSize<Decay<T>>()>::explode(fwd<TF>(mF), fwd<T>(mT));
	}

	#define SSVU_DEFINE_TPLFOR(mType) \
		template<typename TF, typename... TArgs> inline void tplFor(mType mT, TF&& mF) \
			noexcept(noexcept(Internal::ForHelper<TArgs...>::exec(mT, fwd<TF>(mF)))) \
		{ \
			Internal::ForHelper<TArgs...>::exec(mT, fwd<TF>(mF)); \
		}

	SSVU_DEFINE_TPLFOR(std::tuple<TArgs...>&)
	SSVU_DEFINE_TPLFOR(const std::tuple<TArgs...>&)
	SSVU_DEFINE_TPLFOR(std::tuple<TArgs...>&&)

	#undef SSVU_DEFINE_TPLFOR

}

#endif

// TODO: docs

