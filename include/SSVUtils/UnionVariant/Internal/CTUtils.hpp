// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_UNIONVARIANT_CTUTILS
#define SSVU_UNIONVARIANT_CTUTILS

namespace ssvu
{
	namespace Internal
	{
		template<SizeT...> struct CTMax;
		template<SizeT TV0> struct CTMax<TV0> : IntegralConstant<SizeT, TV0> { };
		template<SizeT TV0, SizeT TV1, SizeT... TVs> struct CTMax<TV0, TV1, TVs...> : CTMax<TV0 < TV1 ? TV1 : TV0, TVs...> { };

		template<typename... TArgs> inline constexpr auto getCTMaxSize() noexcept	{ return CTMax<sizeof(TArgs)...>(); }
		template<typename... TArgs> inline constexpr auto getCTMaxAlign() noexcept	{ return CTMax<alignof(TArgs)...>(); }

		template<typename T, typename... TArgs> struct CTHas : TrueT { };
		template<typename T, typename TH, typename... TArgs> struct CTHas<T, TH, TArgs...> : Conditional<isSame<T, TH>(), TrueT, CTHas<T, TArgs...>> { };
		template<typename T> struct CTHas<T> : FalseT { };

		template<typename...> struct PODChecker;
		template<typename T> struct PODChecker<T> : Conditional<isPOD<T>(), TrueT, FalseT> { };
		template<typename T1, typename T2, typename... TArgs> struct PODChecker<T1, T2, TArgs...> : IntegralConstant<bool, PODChecker<T1>() && PODChecker<T2, TArgs...>()> { };
	}
}

#endif
