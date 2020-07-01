// Copyright (c) 2013-2015 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_CORE_COMMON_EMPTYSTRING
#define SSVU_CORE_COMMON_EMPTYSTRING

#include <string>

namespace ssvu
{
/// @brief Returns a const reference to a statically allocated empty
/// `std::string`.
inline const auto& getEmptyStr() noexcept
{
    static std::string result;
    return result;
}
} // namespace ssvu

#endif
