// Copyright (c) 2013-2015 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

// From:
// http://stackoverflow.com/questions/4351371/c-performance-challenge-integer-to-stdstring-conversion
// By: Timo

#ifndef SSVU_CORE_STRING_INTERNAL_FASTINTTOSTR
#define SSVU_CORE_STRING_INTERNAL_FASTINTTOSTR

#include <string>
#include <cstring>
#include <array>


namespace ssvu
{
namespace Impl
{
namespace FastIntToStr
{
template <std::size_t TS>
inline auto& getConvBuffer() noexcept
{
    thread_local std::array<char, TS> result;
    return result;
}

template <typename T, class TEnable = void>
struct Conv;

constexpr char digits[201]{
    "00010203040506070809"
    "10111213141516171819"
    "20212223242526272829"
    "30313233343536373839"
    "40414243444546474849"
    "50515253545556575859"
    "60616263646566676869"
    "70717273747576777879"
    "80818283848586878889"
    "90919293949596979899"};

template <typename T>
struct Conv<T, std::enable_if_t<!std::is_unsigned_v<T>>>
{
    static constexpr std::size_t bufferSize{std::numeric_limits<T>::digits10};

    inline static auto toStr(T val) noexcept
    {
        auto& buf(getConvBuffer<bufferSize>());
        char* it{&buf[bufferSize - 2]};

        if(val >= 0)
        {
            auto div(val / 100);

            while(div)
            {
                std::memcpy(it, &digits[2 * (val - div * 100)], 2);
                val = div;
                it -= 2;
                div = val / 100;
            }

            std::memcpy(it, &digits[2 * val], 2);
            if(val < 10) it++;
        }
        else
        {
            auto div(val / 100);

            while(div)
            {
                std::memcpy(it, &digits[-2 * (val - div * 100)], 2);
                val = div;
                it -= 2;
                div = val / 100;
            }

            std::memcpy(it, &digits[-2 * val], 2);
            if(val <= -10) it--;
            *it = '-';
        }

        return std::string(
            it, (static_cast<char*>(buf.data()) + bufferSize) - it);
    }
};

template <typename T>
struct Conv<T, std::enable_if_t<std::is_unsigned_v<T>>>
{
    static constexpr std::size_t bufferSize{std::numeric_limits<T>::digits10};

    inline static auto toStr(T val) noexcept
    {
        auto& buf(getConvBuffer<bufferSize>());
        char* it{&buf[bufferSize - 2]};

        auto div(val / 100);

        while(div)
        {
            std::memcpy(it, &digits[2 * (val - div * 100)], 2);
            val = div;
            it -= 2;
            div = val / 100;
        }

        std::memcpy(it, &digits[2 * val], 2);
        if(val < 10) it++;

        return std::string(
            it, (static_cast<char*>(buf.data()) + bufferSize) - it);
    }
};

template <typename T>
inline auto toStr(const T& mX) noexcept
{
    return Conv<T>::toStr(mX);
}
} // namespace FastIntToStr
} // namespace Impl
} // namespace ssvu

#endif
