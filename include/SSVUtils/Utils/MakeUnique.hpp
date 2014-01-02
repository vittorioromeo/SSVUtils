// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// Taken from: http://stackoverflow.com/questions/7038357/make-unique-and-perfect-forwarding
// This will be used until C++14 becomes the new standard :)

#ifndef SSVU_UTILS_MAKEUNIQUE
#define SSVU_UTILS_MAKEUNIQUE

#include <memory>
#include <type_traits>
#include <utility>
#include "SSVUtils/Global/Common.hpp"

namespace std
{
	template<typename T, typename... TArgs> inline ssvu::Uptr<T> make_unique_helper(std::false_type, TArgs&&... mArgs) { return ssvu::Uptr<T>(new T(std::forward<TArgs>(mArgs)...)); }
	template<typename T, typename... TArgs> inline ssvu::Uptr<T> make_unique_helper(std::true_type, TArgs&&... mArgs)
	{
		static_assert(std::extent<T>::value == 0, "make_unique<T[N]>() is forbidden, please use make_unique<T[]>().");
		using U = ssvu::RemoveExtent<T>;
		return ssvu::Uptr<T>(new U[sizeof...(TArgs)]{std::forward<TArgs>(mArgs)...});
	}
	template<typename T, typename... TArgs> inline ssvu::Uptr<T> make_unique(TArgs&&... mArgs) { return make_unique_helper<T>(std::is_array<T>(), std::forward<TArgs>(mArgs)...); }
}

#endif

// C++14: remove
