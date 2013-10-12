// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_GLOBAL_TYPEDEFS
#define SSVU_GLOBAL_TYPEDEFS

#include <memory>
#include <functional>
#include <type_traits>

namespace std
{
	template<bool B, class T = void> using enable_if_t = typename std::enable_if<B, T>::type; // C++14: will be in standard
	template<typename... TArgs> using common_type_t = typename std::common_type<TArgs...>::type; // C++14: will be in standard
	template<typename T> using decay_t = typename std::decay<T>::type; // C++14: will be in standard
}

namespace ssvu
{
	template<typename T, typename TDeleter = std::default_delete<T>> using Uptr = std::unique_ptr<T, TDeleter>;
	template<typename... TArgs> using CommonType = std::common_type_t<TArgs...>;
	template<bool TBool, typename T = void> using EnableIfType = std::enable_if_t<TBool, T>;
	template<typename T> using DecayType = std::decay_t<T>;
}

#ifndef SSVU_USE_STD_FUNCTION
	#include "SSVUtils/FastFunc/FastFunc.h"
#endif

namespace ssvu
{
	template<typename> struct FuncHelper;

	#ifndef SSVU_USE_STD_FUNCTION
		template<typename T, typename... TArgs> struct FuncHelper<T(TArgs...)> { using FuncType = FastFunc<T(TArgs...)>; };
	#else
		template<typename T, typename... TArgs> struct FuncHelper<T(TArgs...)> { using FuncType = std::function<T(TArgs...)>; };
	#endif

	template<typename T> using Func = typename FuncHelper<T>::FuncType;

	using Action = Func<void()>;
	using Predicate = Func<bool()>;
}

#endif
