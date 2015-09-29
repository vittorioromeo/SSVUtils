// Copyright (c) 2013-2015 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_HANDLEVECTOR_INTERNAL_COMMON
#define SSVU_HANDLEVECTOR_INTERNAL_COMMON

#include "SSVUtils/Core/Core.hpp"
#include "SSVUtils/Range/Range.hpp"
#include "SSVUtils/GrowableArray/GrowableArray.hpp"

namespace ssvu
{
    /// @typedef HandleVector index type.
    using HIdx = SizeT;

    /// @typedef HandleVector counter type.
    using HCtr = int;

    namespace Impl
    {
        struct HVStat
        {
            HIdx markIdx;
            bool alive{false};

            inline HVStat(HIdx mMarkIdx) noexcept : markIdx{mMarkIdx} {}
        };

        /// @brief Structure controlling validity of the atoms and handles.
        struct HVMark
        {
            HIdx atomIdx;
            HCtr ctr;

            inline HVMark(HIdx mAtomIdx) noexcept : atomIdx{mAtomIdx} {}
        };
    }
}

#endif
