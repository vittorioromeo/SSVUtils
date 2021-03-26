// Copyright (c) 2013-2015 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_CORE_STRING_STRINGIFIERBASE
#define SSVU_CORE_STRING_STRINGIFIERBASE

#include "SSVUtils/Core/ConsoleFmt/ConsoleFmt.hpp"

#include <string>
#include <memory>
#include <sstream>

namespace ssvu
{
namespace Impl
{
template <typename T>
struct StringifierDefault
{
    template <bool TFmt>
    inline static void impl(std::ostream& mStream, const T& mValue)
    {
        mStream << mValue;
    }
};
/// @brief Streams a "formatting reset sequence" to mStream.
inline void resetFmt(std::ostream& mStream)
{
    mStream << Console::resetFmt();
}

inline auto& getStringifyStream() noexcept
{
    thread_local std::ostringstream* oss =
        new std::ostringstream{}; // intentionally leaked

    return *oss;
}
} // namespace Impl

// Stringify common types
template <typename T, typename = void>
struct Stringifier : public Impl::StringifierDefault<T>
{
};

// Stringify C-style strings
template <std::size_t TN>
struct Stringifier<char[TN]> final : public Impl::StringifierDefault<char[TN]>
{
};

template <>
struct Stringifier<char*> final : public Impl::StringifierDefault<char*>
{
};

template <>
struct Stringifier<const char*> final
    : public Impl::StringifierDefault<const char*>
{
};

/// @brief Stringifies a value in a stream.
/// @details Uses Stringifier<T> internally.
/// @tparam TFmt Use formatting? (set to false if the target stream isn't
/// the console)
/// @param mStream Reference to the target stream.
/// @param mValue Const reference to the value. (original value won't be
/// changed)
template <bool TFmt, typename TStream, typename T>
inline void stringify(TStream& mStream, const T& mValue)
{
    Stringifier<T>::template impl<TFmt>(mStream, mValue);
}

namespace Impl
{
template <bool TFmt, bool TResetFmt = true, typename T>
inline void callStringifyImpl(std::ostream& mStream, const T& mValue)
{
    if(TResetFmt) resetFmt(mStream);
    stringify<TFmt>(mStream, mValue);
    if(TResetFmt) resetFmt(mStream);
}
} // namespace Impl

} // namespace ssvu

#endif
