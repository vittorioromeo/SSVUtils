// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// Taken from Andrei Alexandrescu's GoingNative 2013 talk

#ifndef SSVU_CORE_UTILS_TUPLE
#define SSVU_CORE_UTILS_TUPLE

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

#endif

// C++14: replace with STL's solution
