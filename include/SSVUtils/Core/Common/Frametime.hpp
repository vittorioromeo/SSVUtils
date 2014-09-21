// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_CORE_COMMON_FRAMETIME
#define SSVU_CORE_COMMON_FRAMETIME

namespace ssvu
{
	/// @typedef `FT` is an alias for `float` intended to be used in frametime related
	/// contexts. It can be read both as "frametime" and "float time", and should be
	/// used instead of plain float for frametime-driven timers, delays and durations.
	using FT = float;

	/// @typedef `FTDuration` is a millisecond-precision `std::chrono::duration` intended
	/// for use in frametime-related contexts.
	using FTDuration = std::chrono::duration<FT, std::milli>;

	/// @brief Default constexpr ratio between seconds and frametime.
	constexpr float secondsFTRatio{60.f};

	/// @brief Converts frametime to seconds.
	template<typename T> inline constexpr T getFTToSeconds(T mFT) noexcept { return mFT / secondsFTRatio; }

	/// @brief Converts seconds to frametime.
	template<typename T> inline constexpr T getSecondsToFT(T mSeconds) noexcept { return mSeconds * secondsFTRatio; }

	/// @brief Converts frametime to frames per seconds.
	template<typename T> inline constexpr T getFTToFPS(T mFT) noexcept { return secondsFTRatio / mFT; }
}

#endif
