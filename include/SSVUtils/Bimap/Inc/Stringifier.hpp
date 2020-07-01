// Copyright (c) 2013-2015 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_IMPL_BIMAP_STRINGIFIER
#define SSVU_IMPL_BIMAP_STRINGIFIER

#include "SSVUtils/Core/Core.hpp"
#include "SSVUtils/Bimap/Inc/Bimap.hpp"

namespace ssvu
{
// Stringifier support
template <typename T1, typename T2>
struct Stringifier<Bimap<T1, T2>>
{
    template <bool TFmt>
    inline static void impl(std::ostream& mStream, const Bimap<T1, T2>& mValue)
    {
        auto itrBegin(std::begin(mValue));
        if(itrBegin == std::end(mValue))
        {
            Impl::printBold<TFmt>(mStream, "{ EMPTY }");
            return;
        }

        Impl::printBold<TFmt>(mStream, "{");

        Impl::repeatPenultimate(
            itrBegin, std::end(mValue),
            [&mStream](const auto& mE) {
                Impl::callStringifyImpl<TFmt>(mStream, mE->first);
                Impl::printBold<TFmt>(mStream, " <-> ");
                Impl::callStringifyImpl<TFmt>(mStream, mE->second);
            },
            [&mStream](const auto&) { Impl::printBold<TFmt>(mStream, "\n"); });

        Impl::printBold<TFmt>(mStream, "}");
    }
};
} // namespace ssvu

#endif
