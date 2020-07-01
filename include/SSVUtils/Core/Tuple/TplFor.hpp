// Copyright (c) 2013-2015 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_CORE_TUPLE_TPLFOR
#define SSVU_CORE_TUPLE_TPLFOR

#include <tuple>
#include <vrm/pp.hpp>
#include "SSVUtils/Core/Assert/Assert.hpp"
#include "SSVUtils/Core/Common/Common.hpp"
#include "SSVUtils/Core/MPL/MPL.hpp"
#include "SSVUtils/Core/Tuple/Internal/TplForHelper.hpp"

#define SSVU_IMPL_TPLFOR_CALL(mHelper)                              \
    Impl::mHelper<MPL::getMin<std::size_t>(getTplSize<TTpls>()...), \
        TTpls...>::exec(FWD(mF), FWD(mTpls)...)

#define SSVU_IMPL_DEFINE_TPLFOR_FN(mName, mHelper)         \
    template <typename TF, typename... TTpls>              \
    inline void mName(TF&& mF, TTpls&&... mTpls) noexcept( \
        noexcept(SSVU_IMPL_TPLFOR_CALL(mHelper)))          \
    {                                                      \
        SSVU_IMPL_TPLFOR_CALL(mHelper);                    \
    }

namespace ssvu
{
/// @brief Iterates over a tuple's elements passing them to `mF` one at a
/// time.
/// @details Can iterate over multiple tuples at once, passing the Nth
/// element of every tuple to `mF` simultaneously.
/// If the tuples have different sizes, the minimum size will be used.
SSVU_IMPL_DEFINE_TPLFOR_FN(tplFor, ForHelper)

/// @brief Iterates over a tuple's elements passing current iteration data
/// and them to `mF` one at a time.
/// @details Can iterate over multiple tuples at once, passing the Nth
/// element of every tuple to `mF` simultaneously.
/// If the tuples have different sizes, the minimum size will be used.
SSVU_IMPL_DEFINE_TPLFOR_FN(tplForData, ForDataHelper)
} // namespace ssvu

#undef SSVU_IMPL_TPLFOR_CALL
#undef SSVU_IMPL_DEFINE_TPLFOR_FN

#endif
