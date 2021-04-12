// Copyright (c) 2013-2015 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_CORE_STRING_TOSTR
#define SSVU_CORE_STRING_TOSTR

#include "SSVUtils/Core/String/Internal/FastIntToStr.hpp"
#include "SSVUtils/Core/String/StringifierBase.hpp"
#include "SSVUtils/Core/Stringifier/Stringifier.hpp"
#include "SSVUtils/Core/ConsoleFmt/ConsoleFmt.hpp"

#include <string>
#include <memory>
#include <sstream>

namespace ssvu
{

namespace Impl
{
template <typename T>
struct ToStrImpl
{
    inline static auto toStr(const T& mX)
    {
        Impl::getStringifyStream().str("");
        Impl::callStringifyImpl<false, false>(Impl::getStringifyStream(), mX);
        return Impl::getStringifyStream().str();
    }
};

#define SSVU_IMPL_FASTINTTOSTR_CONV(mType)        \
    template <>                                   \
    struct ToStrImpl<mType>                       \
    {                                             \
        inline static auto toStr(const mType& mX) \
        {                                         \
            return Impl::FastIntToStr::toStr(mX); \
        }                                         \
    };

SSVU_IMPL_FASTINTTOSTR_CONV(short)
SSVU_IMPL_FASTINTTOSTR_CONV(int)
SSVU_IMPL_FASTINTTOSTR_CONV(long)
SSVU_IMPL_FASTINTTOSTR_CONV(long long)

SSVU_IMPL_FASTINTTOSTR_CONV(unsigned short)
SSVU_IMPL_FASTINTTOSTR_CONV(unsigned int)
SSVU_IMPL_FASTINTTOSTR_CONV(unsigned long)
SSVU_IMPL_FASTINTTOSTR_CONV(unsigned long long)

#undef SSVU_IMPL_FASTINTTOSTR_CONV
} // namespace Impl

/// @brief Converts a value to a string.
/// @details Uses Stringifier<T> internally, with disabled formatting
/// options.
/// @param mX Const reference to the value. (original value won't be
/// changed)
/// @return Returns a std::string representing the stringified value.
template <typename T>
inline auto toStr(const T& mX)
{
    return Impl::ToStrImpl<T>::toStr(mX);
}

} // namespace ssvu

#endif
