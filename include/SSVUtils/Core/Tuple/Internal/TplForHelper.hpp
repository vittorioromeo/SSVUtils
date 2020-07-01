// Copyright (c) 2013-2015 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_CORE_TUPLE_INTERNAL_TPLFORHELPER
#define SSVU_CORE_TUPLE_INTERNAL_TPLFORHELPER

#include <tuple>
#include "SSVUtils/Core/Common/Common.hpp"

#define SSVU_IMPL_DEFINE_FORHELPER(mName, mBody)                   \
    template <std::size_t TS, typename... TTpls>                   \
    struct mName                                                   \
    {                                                              \
        template <std::size_t TI, typename TF>                     \
        inline static std::enable_if_t<TI == TS, void> exec(       \
            TF, TTpls&&...) noexcept                               \
        {                                                          \
        }                                                          \
                                                                   \
        template <std::size_t TI = 0, typename TF>                 \
            inline static std::enable_if_t <                       \
            TI<TS, void> exec(TF&& mF, TTpls&&... mTpls) noexcept( \
                noexcept(mBody))                                   \
        {                                                          \
            mBody;                                                 \
            exec<TI + 1, TF>(FWD(mF), FWD(mTpls)...);              \
        }                                                          \
    };

namespace ssvu
{
namespace Impl
{
template <typename TTypes, std::size_t TIdx>
struct TplForData
{
    using Types = TTypes;
    inline constexpr auto getIdx() const noexcept
    {
        return TIdx;
    }
};

SSVU_IMPL_DEFINE_FORHELPER(ForHelper, (FWD(mF)(std::get<TI>(FWD(mTpls))...)))
SSVU_IMPL_DEFINE_FORHELPER(ForDataHelper,
    (FWD(mF)(TplForData<MPL::List<decltype(std::get<TI>(FWD(mTpls)))...>, TI>{},
        std::get<TI>(FWD(mTpls))...)))
} // namespace Impl
} // namespace ssvu

#undef SSVU_IMPL_DEFINE_FORHELPER

#endif
