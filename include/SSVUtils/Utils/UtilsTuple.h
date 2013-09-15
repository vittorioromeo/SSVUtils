// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// Taken from Andrei Alexandrescu's GoingNative 2013 talk

#ifndef SSVU_UTILS_TUPLE
#define SSVU_UTILS_TUPLE

#include <tuple>
#include <utility>
#include <typeinfo>

namespace ssvu
{
	namespace Internal
	{
		template<unsigned K, class F, class Tup> struct Expander
		{
			template<class... Us> static auto expand(F&& f, Tup&& t, Us&&... args)
				-> decltype(Expander<K - 1, F, Tup>::expand(std::forward<F>(f), std::forward<Tup>(t), std::get<K - 1>(std::forward<Tup>(t)), std::forward<Us>(args)...))
			{
				return Expander<K - 1, F, Tup>::expand(std::forward<F>(f), std::forward<Tup>(t), std::get<K - 1>(std::forward<Tup>(t)), std::forward<Us>(args)...);
			}
		};

		template<class F, class Tup> struct Expander<0, F, Tup>
		{
			template<class... Us> static auto expand(F&& f, Tup&& t, Us&&... args) -> decltype(f(std::forward<Us>(args)...)) { return f(std::forward<Us>(args)...); }
		};
	}

	template<class F, class... Ts> auto explode(F&& f, const std::tuple<Ts...>& t) -> decltype(Internal::Expander<sizeof...(Ts), F, const std::tuple<Ts...>&>::expand(std::forward<F>(f), t))
	{
		return Internal::Expander<sizeof...(Ts), F, const std::tuple<Ts...>&>::expand(std::forward<F>(f), t);
	}
	template<class F, class... Ts> auto explode(F&& f, std::tuple<Ts...>& t) -> decltype(Internal::Expander<sizeof...(Ts), F, std::tuple<Ts...>&>::expand(std::forward<F>(f), t))
	{
		return Internal::Expander<sizeof...(Ts), F, std::tuple<Ts...>&>::expand(std::forward<F>(f), t);
	}
	template<class F, class... Ts> auto explode(F&& f, std::tuple<Ts...>&& t) -> decltype(Internal::Expander<sizeof...(Ts), F, std::tuple<Ts...>&&>::expand(std::forward<F>(f), std::move(t)))
	{
		return Internal::Expander<sizeof...(Ts), F, std::tuple<Ts...>&&>::expand(std::forward<F>(f), std::move(t));
	}
}

#endif

// C++14: replace with STL's solution
