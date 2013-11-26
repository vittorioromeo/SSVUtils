#ifndef SSVU_UTILS_ITERATOR
#define SSVU_UTILS_ITERATOR

#include <iterator>

namespace std
{
	template<typename T> inline auto cbegin(const T& mContainer) noexcept -> decltype(std::begin(mContainer))	{ return std::begin(mContainer); }
	template<typename T> inline auto cend(const T& mContainer) noexcept -> decltype(std::end(mContainer))		{ return std::end(mContainer); }
	template<typename T> inline auto rbegin(const T& mContainer) noexcept -> decltype(mContainer.rbegin())		{ return mContainer.rbegin(); }
	template<typename T> inline auto rbegin(T& mContainer) noexcept -> decltype(mContainer.rbegin())			{ return mContainer.rbegin(); }
	template<typename T> inline auto crbegin(const T& mContainer) noexcept -> decltype(rbegin(mContainer))		{ return rbegin(mContainer); }
	template<typename T> inline auto rend(const T& mContainer) noexcept -> decltype(mContainer.rend())			{ return mContainer.rend(); }
	template<typename T> inline auto rend(T& mContainer) noexcept -> decltype(mContainer.rend())				{ return mContainer.rend(); }
	template<typename T> inline auto crend(const T& mContainer) noexcept -> decltype(rend(mContainer))			{ return rend(mContainer); }
}

#endif

// C++14: remove, will be obsolete
