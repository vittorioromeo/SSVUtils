// Copyright (c) 2013-2015 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_CORE_LOG_INTERNAL_IMPL_ENABLED
#define SSVU_CORE_LOG_INTERNAL_IMPL_ENABLED

#include "SSVUtils/Core/ConsoleFmt/ConsoleFmt.hpp"
#include "SSVUtils/Core/String/IgnoreManip.hpp"

#include <string>
#include <mutex>
#include <unordered_map>
#include <iomanip>

namespace ssvu
{
namespace Impl
{
/// @brief Returns an unique color based of `mStr`'s hash.
inline const auto& getUniqueColor(const std::string& mStr)
{
    static std::unordered_map<std::string, Console::Color> map;

    if(map.count(mStr) == 0)
    {
        static int lastColorIdx{2};
        map[mStr] = Console::Color(getMod(lastColorIdx++, 2, 7));
    }

    return Console::setColorFG(map[mStr]);
}

/// @brief Implementation of the "cout-like" `lo()` object type.
struct LOut
{
    static constexpr std::size_t leftW{38};

    std::mutex mtx;
    std::string title;
    std::ostream& stream;

    inline LOut(std::ostream& mStream) noexcept : stream{mStream}
    {
    }

    inline void flush()
    {
        std::lock_guard<std::mutex> SSVU_UNIQUE_NAME{mtx};

        stream.flush();
        getLogStream().flush();
    }
};

/// @brief Returns a reference to the statically allocated global `LOut`
/// instance.
inline auto& lo() noexcept
{
    static LOut result{std::cout};
    return result;
}

/// @brief Interaction between the `lo()` object and a "stringificable"
/// object.
template <typename T>
inline auto& operator<<(LOut& mLOut, const T& mValue)
{
    std::lock_guard<std::mutex> SSVU_UNIQUE_NAME{lo().mtx};

    if(!getLogSuppressed())
    {
        if(!mLOut.title.empty())
        {
            auto tStr("[" + mLOut.title + "] ");
            std::cout << getUniqueColor(tStr)
                      << Console::setStyle(Console::Style::Bold) << std::left
                      << std::setw(LOut::leftW) << tStr;
            getLogStream() << std::left << std::setw(LOut::leftW) << tStr;
            mLOut.title.clear();
        }

        mLOut.stream << Console::resetFmt();
        stringify<true>(mLOut.stream, mValue);
        mLOut.stream << Console::resetFmt();

        stringify<false>(getLogStream(), mValue);
    }

    return mLOut;
}

/// @brief Interaction between the `lo()` object and a stream
/// manipulator.
inline auto& operator<<(LOut& mLOut, StdEndLine mManip)
{
    std::lock_guard<std::mutex> SSVU_UNIQUE_NAME{lo().mtx};

    mManip(std::cout);
    mManip(getLogStream());
    return mLOut;
}

/// @brief Interaction between the `lo()` object and a `IgnoreManip`
/// object.
inline auto& operator<<(LOut& mLOut, const IgnoreManip& mIBM)
{
    for(auto c : mIBM) std::cout.put(c);
    return mLOut;
}

/// @brief Sets `lo()`'s current title and returns a reference to it.
template <typename T>
inline LOut& lo(const T& mTitle)
{
    if(!getLogSuppressed())
    {
        std::lock_guard<std::mutex> SSVU_UNIQUE_NAME{lo().mtx};

        lo().title = toStr(mTitle);
    }

    return lo();
}

/// @brief Statically allocated horizontal ruler implementation.
inline const char* hr() noexcept
{
    static std::string str(Console::Info::getColumnCount(), '_');
    return str.c_str();
}

/// @brief Dynamic horizontal ruler implementation.
inline std::string hr(int mOffset, char mChar)
{
    return std::string(Console::Info::getColumnCount() + mOffset, mChar);
}
} // namespace Impl
} // namespace ssvu

#endif

// TODO: investigate possibility of counting characters printed in a certain
// line in order to create automatic-length hr()
