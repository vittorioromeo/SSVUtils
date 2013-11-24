#ifndef SSVU_UTILS_ITERATOR
#define SSVU_UTILS_ITERATOR

#include <iterator>

namespace std
{
	template<typename T> auto cbegin(const T& mContainer) noexcept -> decltype(std::begin(mContainer)) 	{ return std::begin(mContainer); }
	template<typename T> auto cend(const T& mContainer) noexcept -> decltype(std::end(mContainer))		{ return std::end(mContainer); }
	template<typename T> auto rbegin(const T& mContainer) noexcept -> decltype(mContainer.rbegin())		{ return mContainer.rbegin(); }
	template<typename T> auto rbegin(T& mContainer) noexcept -> decltype(mContainer.rbegin())			{ return mContainer.rbegin(); }
	template<typename T> auto crbegin(const T& mContainer) noexcept -> decltype(rbegin(mContainer))		{ return rbegin(mContainer); }
	template<typename T> auto rend(const T& mContainer) noexcept -> decltype(mContainer.rend())			{ return mContainer.rend(); }
	template<typename T> auto rend(T& mContainer) noexcept -> decltype(mContainer.rend())				{ return mContainer.rend(); }
	template<typename T> auto crend(const T& mContainer) noexcept -> decltype(rend(mContainer))			{ return rend(mContainer); }
}

#endif

// C++14: remove, will be obsolete
