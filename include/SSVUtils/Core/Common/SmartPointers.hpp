// Copyright (c) 2013-2015 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_CORE_COMMON_SMARTPOINTERS
#define SSVU_CORE_COMMON_SMARTPOINTERS

#include "SSVUtils/Core/Common/Aliases.hpp"

#include <memory>

namespace ssvu
{
namespace Impl
{

/// @brief Internal functor that creates an `std::unique_ptr`.
template <typename T>
struct MakerUPtr
{
    template <typename... TArgs>
    inline static auto make(TArgs&&... mArgs)
    {
        return std::make_unique<T>(FWD(mArgs)...);
    }
};

} // namespace Impl
} // namespace ssvu

#endif
