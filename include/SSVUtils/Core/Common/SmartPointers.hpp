// Copyright (c) 2013-2015 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_CORE_COMMON_SMARTPOINTERS
#define SSVU_CORE_COMMON_SMARTPOINTERS

#include "SSVUtils/Core/Common/Aliases.hpp"

namespace ssvu
{
/// @brief Creates and returns an `std::unique_ptr<T>`.
/// @details Wraps `std::make_unique<T>`.
template <typename T, typename... TArgs>
inline decltype(auto) mkUPtr(TArgs&&... mArgs)
{
    return std::make_unique<T>(FWD(mArgs)...);
}

/// @brief Creates and returns an `std::shared_ptr<T>`.
/// @details Wraps `std::make_shared<T>`.
template <typename T, typename... TArgs>
inline decltype(auto) mkSPtr(TArgs&&... mArgs)
{
    return std::make_shared<T>(FWD(mArgs)...);
}

namespace Impl
{
/// @brief Internal functor that creates an `std::unique_ptr`.
template <typename T>
struct MakerUPtr
{
    template <typename... TArgs>
    inline static auto make(TArgs&&... mArgs)
    {
        return mkUPtr<T>(FWD(mArgs)...);
    }
};

/// @brief Internal functor that creates an `std::shared_ptr`.
template <typename T>
struct MakerSPtr
{
    template <typename... TArgs>
    inline static auto make(TArgs&&... mArgs)
    {
        return mkSPtr<T>(FWD(mArgs)...);
    }
};
} // namespace Impl
} // namespace ssvu

#endif
