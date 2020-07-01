// Copyright (c) 2013-2015 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_CORE_CONSOLEFMT_INTERNAL_IMPL
#define SSVU_CORE_CONSOLEFMT_INTERNAL_IMPL

#include "SSVUtils/Core/Common/EmptyString.hpp"

// This file contains a null implementation for console formatting.
// Everything returns an empty string.

namespace ssvu
{
namespace Console
{
namespace Impl
{
inline const auto& getStrResetFmt() noexcept
{
    return getEmptyStr();
}
inline const auto& getStrStyle(Style) noexcept
{
    return getEmptyStr();
}
inline const auto& getStrColorFG(Color) noexcept
{
    return getEmptyStr();
}
inline const auto& getStrColorBG(Color) noexcept
{
    return getEmptyStr();
}
inline const auto& getStrClear() noexcept
{
    return getEmptyStr();
}
inline bool isInfoValid() noexcept
{
    return false;
}

namespace Info
{
inline std::size_t getColumnCount() noexcept
{
    return 80;
}
inline std::size_t getRowCount() noexcept
{
    return 50;
}
} // namespace Info
} // namespace Impl
} // namespace Console
} // namespace ssvu

#endif
