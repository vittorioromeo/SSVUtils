// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_GLOBAL_TYPEDEFS
#define SSVU_GLOBAL_TYPEDEFS

#include <memory>
#include <functional>
#include "SSVUtils/Delegate/FastFunc.h"

namespace ssvu
{
	template<typename T, typename TDeleter = std::default_delete<T>> using Uptr = std::unique_ptr<T, TDeleter>;

	template<typename TSignature> struct FuncHelper;
	template<typename T, typename... TArgs> struct FuncHelper<T(TArgs...)> { using FuncType = FastFunc<T(TArgs...)>; };
	template<typename T> using Func = typename FuncHelper<T>::FuncType;

	using Action = Func<void()>;
	using Predicate = Func<bool()>;
}

#endif
