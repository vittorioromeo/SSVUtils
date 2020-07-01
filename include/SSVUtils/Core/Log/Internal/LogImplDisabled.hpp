// Copyright (c) 2013-2015 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_CORE_LOG_INTERNAL_IMPL_DISABLED
#define SSVU_CORE_LOG_INTERNAL_IMPL_DISABLED

#include "SSVUtils/Core/Common/EmptyString.hpp"

#include <string>

namespace ssvu
{
namespace Impl
{
inline const auto& getUniqueColor(const std::string&)
{
    return Console::setColorFG(Console::Color::Default);
}

using LOut = NullLOut;

inline LOut& lo() noexcept
{
    static LOut result;
    return result;
}
template <typename T>
inline LOut& lo(const T&)
{
    return lo();
}

inline const char* hr() noexcept
{
    return getEmptyStr().c_str();
}
inline std::string hr(int, char)
{
    return getEmptyStr();
}
} // namespace Impl
} // namespace ssvu

#endif
