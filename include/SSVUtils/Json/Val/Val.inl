// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_JSON_VALUE_INL
#define SSVU_JSON_VALUE_INL

namespace ssvu
{
	namespace Json
	{
		inline auto& Val::operator=(const Val& mV) noexcept	{ set(mV); return *this; }
		inline auto& Val::operator=(Val&& mV) noexcept		{ set(std::move(mV)); return *this; }

		template<typename T> inline auto Val::as() & -> decltype(Internal::AsHelper<T>::as(*this))		{ SSVU_ASSERT(isNoNum<T>()); return Internal::AsHelper<T>::as(*this); }
		template<typename T> inline auto Val::as() const& -> decltype(Internal::AsHelper<T>::as(*this))	{ SSVU_ASSERT(isNoNum<T>()); return Internal::AsHelper<T>::as(*this); }
		template<typename T> inline auto Val::as() && -> decltype(Internal::AsHelper<T>::as(*this))		{ SSVU_ASSERT(isNoNum<T>()); return std::move(Internal::AsHelper<T>::as(*this)); }

		template<typename TWS> inline void Val::writeToStream(std::ostream& mStream) const { Internal::Writer<TWS> w; w.write(*this, mStream); mStream.flush(); }
		template<typename TRS, typename T> inline void Val::readFromStr(T&& mStr) { Internal::Reader<TRS> r{fwd<T>(mStr)}; Internal::tryParse<TRS>(*this, r); }

		inline auto Val::forUncheckedObj() noexcept			{ return forUncheckedObjAs<Val>(); }
		inline auto Val::forUncheckedObj() const noexcept	{ return forUncheckedObjAs<Val>(); }
		inline auto Val::forUncheckedArr() noexcept			{ return forUncheckedArrAs<Val>(); }
		inline auto Val::forUncheckedArr() const noexcept	{ return forUncheckedArrAs<Val>(); }

		inline auto Val::forObj() noexcept			{ return forObjAs<Val>(); }
		inline auto Val::forObj() const noexcept	{ return forObjAs<Val>(); }
		inline auto Val::forArr() noexcept			{ return forArrAs<Val>(); }
		inline auto Val::forArr() const noexcept	{ return forArrAs<Val>(); }
	}
}

#endif

