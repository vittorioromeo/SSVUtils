// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_JSON_VALUE_INL
#define SSVU_JSON_VALUE_INL

namespace ssvu
{
	namespace Json
	{
		template<typename T> inline decltype(auto) Val::as() &// noexcept(noexcept(Internal::AsHelper<T>::as(std::declval<Val&>())))
		{
			SSVU_ASSERT(is<T>());
			return Internal::AsHelper<T>::as(*this);
		}
		template<typename T> inline decltype(auto) Val::as() const&// noexcept(noexcept(Internal::AsHelper<T>::as(std::declval<Val&>())))
		{
			SSVU_ASSERT(is<T>());
			return Internal::AsHelper<T>::as(*this);
		}
		template<typename T> inline decltype(auto) Val::as() &&// noexcept(noexcept(Internal::AsHelper<T>::as(std::declval<Val&&>())))
		{
			SSVU_ASSERT(is<T>());
			return Internal::AsHelper<T>::as(std::move(*this));
		}


		template<typename TWS> inline void Val::writeToStream(std::ostream& mStream) const { Internal::Writer<TWS> w; w.write(*this, mStream); mStream.flush(); }
		template<typename TRS, typename T> inline void Val::readFromStr(T&& mStr) { Internal::Reader<TRS> r{fwd<T>(mStr)}; Internal::tryParse<TRS>(*this, r); }
	}
}

#endif
