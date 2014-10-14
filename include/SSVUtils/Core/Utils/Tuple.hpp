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
			template<typename TF, typename T, typename... TArgs> inline static auto explode(TF&& mF, T&& mT, TArgs&&... mArgs)
			{
				return Exploder<N - 1>::explode(fwd<TF>(mF), fwd<T>(mT), ::std::get<N - 1>(fwd<T>(mT)), fwd<TArgs>(mArgs)...);
			}
		};

		template<> struct Exploder<0>
		{
			template<typename TF, typename T, typename... TArgs> inline static auto explode(TF&& mF, T&&, TArgs&&... mArgs)
			{
				return fwd<TF>(mF)(fwd<TArgs>(mArgs)...);
			}
		};
	}

	template<typename TF, typename T> inline auto explode(TF&& mF, T&& mT)
	{
		return Internal::Exploder<getTupleSize<Decay<T>>()>::explode(fwd<TF>(mF), fwd<T>(mT));
	}
}

#endif

