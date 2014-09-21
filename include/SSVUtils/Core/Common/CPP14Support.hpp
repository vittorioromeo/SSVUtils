// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_CORE_COMMON_CPP14SUPPORT
#define SSVU_CORE_COMMON_CPP14SUPPORT

namespace std
{
	// C++14: will be in standard (should be?)
	template<typename T> inline auto rbegin(const T& mContainer) noexcept	{ return mContainer.rbegin(); }
	template<typename T> inline auto rbegin(T& mContainer) noexcept 		{ return mContainer.rbegin(); }
	template<typename T> inline auto crbegin(const T& mContainer) noexcept	{ return rbegin(mContainer); }
	template<typename T> inline auto rend(const T& mContainer) noexcept 	{ return mContainer.rend(); }
	template<typename T> inline auto rend(T& mContainer) noexcept 			{ return mContainer.rend(); }
	template<typename T> inline auto crend(const T& mContainer) noexcept 	{ return rend(mContainer); }
}

#endif
