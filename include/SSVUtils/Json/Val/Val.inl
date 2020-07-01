// Copyright (c) 2013-2015 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_JSON_VALUE_INL
#define SSVU_JSON_VALUE_INL

#include "SSVUtils/Json/Val/Val.hpp"
#include "SSVUtils/Json/Io/Io.hpp"
#include "SSVUtils/Json/Val/Internal/Cnv.hpp"
#include "SSVUtils/Json/Val/Internal/CnvFuncs.hpp"
#include "SSVUtils/Json/Val/Internal/CnvMacros.hpp"
#include "SSVUtils/Json/Val/Internal/Chk.hpp"
#include "SSVUtils/Json/Val/Internal/AsHelper.hpp"

namespace ssvu
{
namespace Json
{
inline Val& Val::operator=(const Val& mV) noexcept
{
    set(mV);
    return *this;
}
inline Val& Val::operator=(Val&& mV) noexcept
{
    set(std::move(mV));
    return *this;
}

template <typename T>
inline auto Val::as() & -> decltype(Impl::AsHelper<T>::as(*this))
{
    SSVU_ASSERT(isNoNum<T>());
    return Impl::AsHelper<T>::as(*this);
}
template <typename T>
inline auto Val::as() const& -> decltype(Impl::AsHelper<T>::as(*this))
{
    SSVU_ASSERT(isNoNum<T>());
    return Impl::AsHelper<T>::as(*this);
}
template <typename T>
inline auto Val::as() && -> decltype(Impl::AsHelper<T>::as(*this))
{
    SSVU_ASSERT(isNoNum<T>());
    return std::move(Impl::AsHelper<T>::as(*this));
}

template <typename TWS>
inline void Val::writeToStream(std::ostream& mStream) const
{
    Impl::Writer<TWS> w;
    w.write(*this, mStream);
    mStream.flush();
}
template <typename TRS, typename T>
inline void Val::readFromStr(T&& mStr)
{
    Impl::Reader<TRS> r{FWD(mStr)};
    Impl::tryParse<TRS>(*this, r);
}

inline auto Val::forUncheckedObj() noexcept
{
    return forUncheckedObjAs<Val>();
}
inline auto Val::forUncheckedObj() const noexcept
{
    return forUncheckedObjAs<Val>();
}
inline auto Val::forUncheckedArr() noexcept
{
    return forUncheckedArrAs<Val>();
}
inline auto Val::forUncheckedArr() const noexcept
{
    return forUncheckedArrAs<Val>();
}

inline auto Val::forObj() noexcept
{
    return forObjAs<Val>();
}
inline auto Val::forObj() const noexcept
{
    return forObjAs<Val>();
}
inline auto Val::forArr() noexcept
{
    return forArrAs<Val>();
}
inline auto Val::forArr() const noexcept
{
    return forArrAs<Val>();
}
} // namespace Json
} // namespace ssvu

#endif
