// Copyright (c) 2013-2015 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_CORE_UTILS_RND
#define SSVU_CORE_UTILS_RND

#include <random>
#include <vrm/pp.hpp>
#include "SSVUtils/Core/Assert/Assert.hpp"
#include "SSVUtils/Core/Common/Common.hpp"
#include "SSVUtils/Internal/PCG/PCG.hpp"

namespace ssvu
{
namespace Impl
{
    struct PCGInit
    {
        pcg_extras::seed_seq_from<std::random_device> seedSrc;
        pcg32_fast rng{seedSrc};
    };
}

inline auto& getRndEngine() noexcept
{
    thread_local Impl::PCGInit pcgInit;
    return pcgInit.rng;
}

template <typename T>
using RndDistUniformI = std::uniform_int_distribution<T>;
template <typename T>
using RndDistUniformR = std::uniform_real_distribution<T>;
template <typename T>
using RndDistNormalR = std::normal_distribution<T>;

/// @brief Returns a random integer value between [mMin and mMax). (Uniform
/// distribution)
/// @param mMin Lower inclusive bound.
/// @param mMax Upper exclusive bound.
template <typename T1, typename T2>
inline auto getRndI(const T1& mMin, const T2& mMax) noexcept
{
    using CT = Common<T1, T2>;
    CT min(mMin), max(mMax);

    SSVU_ASSERT(min < max);
    return RndDistUniformI<CT>{min, max - 1}(getRndEngine());
}

/// @brief Returns a random real value between [mMin and mMax]. (Uniform
/// distribution)
/// @param mMin Lower inclusive bound.
/// @param mMax Upper inclusive bound.
template <typename T1, typename T2>
inline auto getRndR(const T1& mMin, const T2& mMax) noexcept
{
    using CT = Common<T1, T2>;
    CT min(mMin), max(mMax);

    SSVU_ASSERT(mMin <= mMax);
    return RndDistUniformR<CT>(min, max)(getRndEngine());
}

/// @brief Returns a random real value. (Normal distribution)
/// @tparam T Type of real value. (default float)
/// @param mMean Mean of the distribution.
/// @param mDeviation Deviation of the distribution.
template <typename T1, typename T2>
inline auto getRndRNormal(const T1& mMean, const T2& mDeviation) noexcept
{
    using CT = Common<T1, T2>;
    CT mean(mMean), deviation(mDeviation);

    return RndDistNormalR<CT>(mean, deviation)(getRndEngine());
}

/// @brief Gets a random sign.
/// @tparam T Type of integer value. (default int)
/// @return Returns -1 or 1.
template <typename T = int>
inline T getRndSign() noexcept
{
    return RndDistUniformI<T>{0, 1}(getRndEngine()) > 0 ? -1 : 1;
}
}

#endif
