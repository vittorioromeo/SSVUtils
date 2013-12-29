// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// Taken from Andrei Alexandrescu's GoingNative 2013 talk

#ifndef SSVU_UTILS_TUPLE
#define SSVU_UTILS_TUPLE

#include <tuple>
#include <utility>
#include <typeinfo>
#include <type_traits>

namespace ssvu
{
	namespace Internal
	{
		template<unsigned K, typename F, typename Tup> struct Expander
		{
			template<typename... Us> inline constexpr static auto expand(F&& f, Tup&& t, Us&&... args)
				-> decltype(Expander<K - 1, F, Tup>::expand(std::forward<F>(f), std::forward<Tup>(t), std::get<K - 1>(std::forward<Tup>(t)), std::forward<Us>(args)...))
			{
				return Expander<K - 1, F, Tup>::expand(std::forward<F>(f), std::forward<Tup>(t), std::get<K - 1>(std::forward<Tup>(t)), std::forward<Us>(args)...);
			}
		};

		template<typename F, typename Tup> struct Expander<0, F, Tup>
		{
			template<typename... TArgs> inline constexpr static auto expand(F&& f, Tup&&, TArgs&&... mArgs) -> decltype(f(std::forward<TArgs>(mArgs)...)) { return f(std::forward<TArgs>(mArgs)...); }
		};

		template<std::size_t TIdx, typename TSearch, typename First, typename... TArgs> struct GetImpl
		{
			using type = typename GetImpl<TIdx + 1, TSearch, TArgs...>::type;
			static constexpr int idx{TIdx};
		};

		template<std::size_t TIdx, typename TSearch, typename... TArgs> struct GetImpl<TIdx, TSearch, TSearch, TArgs...>
		{
			using type = GetImpl;
			static constexpr int idx{TIdx};
		};
	}

	template<typename F, typename... TArgs> inline constexpr auto explode(F&& f, const std::tuple<TArgs...>& t) -> decltype(Internal::Expander<sizeof...(TArgs), F, const std::tuple<TArgs...>&>::expand(std::forward<F>(f), t))
	{
		return Internal::Expander<sizeof...(TArgs), F, const std::tuple<TArgs...>&>::expand(std::forward<F>(f), t);
	}
	template<typename F, typename... TArgs> inline constexpr auto explode(F&& f, std::tuple<TArgs...>& t) -> decltype(Internal::Expander<sizeof...(TArgs), F, std::tuple<TArgs...>&>::expand(std::forward<F>(f), t))
	{
		return Internal::Expander<sizeof...(TArgs), F, std::tuple<TArgs...>&>::expand(std::forward<F>(f), t);
	}
	template<typename F, typename... TArgs> inline constexpr auto explode(F&& f, std::tuple<TArgs...>&& t) -> decltype(Internal::Expander<sizeof...(TArgs), F, std::tuple<TArgs...>&&>::expand(std::forward<F>(f), std::move(t)))
	{
		return Internal::Expander<sizeof...(TArgs), F, std::tuple<TArgs...>&&>::expand(std::forward<F>(f), std::move(t));
	}
}

namespace std
{
	template<typename T, typename... TArgs> inline auto get(const std::tuple<TArgs...>& mTpl) noexcept
		-> decltype(std::get<ssvu::Internal::GetImpl<0, T, TArgs...>::type::idx>(mTpl))
	{
		return std::get<ssvu::Internal::GetImpl<0, T, TArgs...>::type::idx>(mTpl);
	}
}

#endif

// C++14: replace with STL's solution
// C++14: remove get
