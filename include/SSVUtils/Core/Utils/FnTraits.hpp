// Copyright (c) 2013-2015 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_CORE_UTILS_FNTRAITS
#define SSVU_CORE_UTILS_FNTRAITS

#include "SSVUtils/Core/Common/Common.hpp"

namespace ssvu
{
	template<typename T> struct FnTraits : public FnTraits<decltype(&T::operator())> { };

	namespace Impl
	{
		template<typename C, typename R, typename... A> struct MemFnType
		{
			using Type = Conditional<
				isConst<C>(),
				Conditional<
					isVolatile<C>(),
					R(C::*)(A...) const volatile,
					R(C::*)(A...) const
				>,
				Conditional<
					isVolatile<C>(),
					R(C::*)(A...) volatile,
					R(C::*)(A...)
				>
			>;
		};
	}

	template<typename TR, typename... TArgs> struct FnTraits<TR(TArgs...)>
	{
		/// @brief Type returned by the function.
		using Return = TR;

		/// @brief Function type.
		using Type = TR(TArgs...);

		/// @brief Function type as if this function were a member function of the `TOwner` class.
		template<typename TOwner> using MemFnType = typename Impl::MemFnType<RmPtr<RmRef<TOwner>>, TR, TArgs...>::Type;

		/// @brief Arity of the function.
		static constexpr SizeT arity{sizeof...(TArgs)};

		/// @brief Type of the n-th argument of the function.
		template<SizeT TI> using Arg = TplElem<TI, Tpl<TArgs...>>;
	};

	// Match function pointers
	template<typename TR, typename... TArgs> struct FnTraits<TR(*)(TArgs...)> : public FnTraits<TR(TArgs...)> { };

	// Match member functions
	template<typename TC, typename TR, typename... TArgs>	struct FnTraits<TR(TC::*)(TArgs...)>				: public FnTraits<TR(TArgs...)> { using Owner = TC&; };
	template<typename TC, typename TR, typename... TArgs>	struct FnTraits<TR(TC::*)(TArgs...) const>			: public FnTraits<TR(TArgs...)> { using Owner = const TC&; };
	template<typename TC, typename TR, typename... TArgs>	struct FnTraits<TR(TC::*)(TArgs...) volatile>		: public FnTraits<TR(TArgs...)> { using Owner = volatile TC&; };
	template<typename TC, typename TR, typename... TArgs>	struct FnTraits<TR(TC::*)(TArgs...) const volatile>	: public FnTraits<TR(TArgs...)> { using Owner = const volatile TC&; };

	// Match `Func`
	template<typename TF> struct FnTraits<Func<TF>> : public FnTraits<TF> { };

	// Match member functions using `mem_fn`
	#if defined(_GLIBCXX_FUNCTIONAL)
		#define SSVU_IMPL_MEM_FN std::_Mem_fn
	#elif defined(_LIBCPP_FUNCTIONAL)
		#define SSVU_IMPL_MEM_FN std::__mem_fn
	#endif

	template<typename R, typename C> 				struct FnTraits<SSVU_IMPL_MEM_FN<R C::*>>						: public FnTraits<R(C*)> { };
	template<typename R, typename C, typename... A>	struct FnTraits<SSVU_IMPL_MEM_FN<R(C::*)(A...)>>				: public FnTraits<R(C*, A...)> { };
	template<typename R, typename C, typename... A>	struct FnTraits<SSVU_IMPL_MEM_FN<R(C::*)(A...) const>>			: public FnTraits<R(const C*, A...)> { };
	template<typename R, typename C, typename... A>	struct FnTraits<SSVU_IMPL_MEM_FN<R(C::*)(A...) volatile>>		: public FnTraits<R(volatile C*, A...)> { };
	template<typename R, typename C, typename... A>	struct FnTraits<SSVU_IMPL_MEM_FN<R(C::*)(A...) const volatile>>	: public FnTraits<R(const volatile C*, A...)> { };

	#ifdef SSVU_IMPL_MEM_FN
		#undef SSVU_IMPL_MEM_FN
	#endif

	// Match qualified functions
	template<typename T> struct FnTraits<T&>					: public FnTraits<T> { };
	template<typename T> struct FnTraits<const T&>				: public FnTraits<T> { };
	template<typename T> struct FnTraits<volatile T&>			: public FnTraits<T> { };
	template<typename T> struct FnTraits<const volatile T&>		: public FnTraits<T> { };
	template<typename T> struct FnTraits<T&&>					: public FnTraits<T> { };
	template<typename T> struct FnTraits<const T&&>				: public FnTraits<T> { };
	template<typename T> struct FnTraits<volatile T&&>			: public FnTraits<T> { };
	template<typename T> struct FnTraits<const volatile T&&>	: public FnTraits<T> { };
}

#endif
