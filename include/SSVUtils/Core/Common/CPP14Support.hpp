// Copyright (c) 2013-2015 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_CORE_COMMON_CPP14SUPPORT
#define SSVU_CORE_COMMON_CPP14SUPPORT

using namespace std::literals;
using namespace std::string_literals;
using namespace std::chrono_literals;

namespace std
{
	// TODO: GCC: BUG: C++14: will be in standard (should be?)
	// libc++ has these in stndard, but not libstdc++

	#if defined(SSVU_STDLIB_LIBSTDCXX)
		template<typename TC> inline auto cbegin(const TC& mContainer) noexcept		{ return mContainer.cbegin(); }
		template<typename TC> inline auto rbegin(const TC& mContainer) noexcept		{ return mContainer.rbegin(); }
		template<typename TC> inline auto rbegin(TC& mContainer) noexcept			{ return mContainer.rbegin(); }
		template<typename TC> inline auto crbegin(const TC& mContainer) noexcept	{ return rbegin(mContainer); }
		template<typename TC> inline auto cend(const TC& mContainer) noexcept		{ return mContainer.cend(); }
		template<typename TC> inline auto rend(const TC& mContainer) noexcept		{ return mContainer.rend(); }
		template<typename TC> inline auto rend(TC& mContainer) noexcept				{ return mContainer.rend(); }
		template<typename TC> inline auto crend(const TC& mContainer) noexcept		{ return rend(mContainer); }
	#endif
}

#endif
