// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_CORE_UTILS_MATH
#define SSVU_CORE_UTILS_MATH

namespace ssvu
{
	inline std::minstd_rand& getRndEngine()	noexcept { static std::minstd_rand rndEngine(std::time(0)); return rndEngine; }
	template<typename T> using RndDistributionI = std::uniform_int_distribution<T>;
	template<typename T> using RndDistributionR = std::uniform_real_distribution<T>;

	// C++14: templatized values (GCC 5?)
	/// @brief Value of pi. (pi radians = 180 degrees)
	constexpr float pi{3.14159265f};

	/// @brief Value of pi / 2. (half pi radians = 90 degrees)
	constexpr float piHalf{pi / 2.f};

	/// @brief Value of pi * 2. (tau radians = 360 degrees)
	constexpr float tau{pi * 2.f};

	/// @brief Ratio between radians and degrees.
	constexpr float radDegRatio{pi / 180.f};

	/// @brief Gets a random integer value between [mMin and mMax).
	/// @tparam T Type of integer value. (default int)
	/// @param mMin Lower inclusive bound.
	/// @param mMax Upper exclusive bound.
	/// @return Returns a random integer value, between [mMin and mMax).
	template<typename T = int> inline T getRnd(const T& mMin, const T& mMax) { SSVU_ASSERT(mMin < mMax); return RndDistributionI<T>(mMin, mMax - 1)(getRndEngine()); }

	/// @brief Gets a random real value between [mMin and mMax].
	/// @tparam T Type of real value. (default float)
	/// @param mMin Lower inclusive bound.
	/// @param mMax Upper inclusive bound.
	/// @return Returns a random real value, between [mMin and mMax].
	template<typename T = float> inline T getRndR(const T& mMin, const T& mMax) { SSVU_ASSERT(mMin <= mMax); return RndDistributionR<T>{mMin, mMax}(getRndEngine()); }

	/// @brief Gets a random sign.
	/// @tparam T Type of integer value. (default int)
	/// @return Returns -1 or 1.
	template<typename T = int> inline T getRndSign() { return RndDistributionI<T>{0, 1}(getRndEngine()) > 0 ? -1 : 1; }

	/// @brief Gets the sign of a numeric value. (unsigned version)
	/// @param mValue Value to use.
	/// @return Returns 1 if the value is >0, -1 if the value is < 0, 0 if the value == 0.
	template<typename T> inline constexpr int getSign(const T& mValue, EnableIf<!isSigned<T>()>* = nullptr) noexcept { return 0 < mValue; }

	/// @brief Gets the sign of a numeric value. (signed version)
	/// @param mValue Value to use.
	/// @return Returns 1 if the value is >0, -1 if the value is < 0, 0 if the value == 0.
	template<typename T> inline constexpr int getSign(const T& mValue, EnableIf<isSigned<T>()>* = nullptr) noexcept { return (0 < mValue) - (mValue < 0); }

	/// @brief Clamps a numeric value. (lower-bound only)
	/// @param mValue Reference to the value. (will be modified)
	/// @param mMin Lower bound.
	template<typename T1, typename T2> inline void clampMin(T1& mValue, const T2& mMin) noexcept { if(mValue < mMin) mValue = mMin; }

	/// @brief Clamps a numeric value. (upper-bound only)
	/// @param mValue Reference to the value. (will be modified)
	/// @param mMax Upper bound.
	template<typename T1, typename T2> inline void clampMax(T1& mValue, const T2& mMax) noexcept { if(mValue > mMax) mValue = mMax; }

	/// @brief Clamps a numeric value.
	/// @param mValue Reference to the value. (will be modified)
	/// @param mMin Lower bound.
	/// @param mMax Upper bound.
	template<typename T1, typename T2, typename T3> inline void clamp(T1& mValue, const T2& mMin, const T3& mMax) noexcept
	{
		SSVU_ASSERT(mMin <= mMax);
		if(mValue < mMin) mValue = mMin;
		else if(mValue > mMax) mValue = mMax;
	}

	/// @brief Gets a clamped numeric value. (lower-bound only)
	/// @param mValue Const reference to the value. (original value won't be changed)
	/// @param mMin Lower bound.
	/// @return Returns mMin if mValue < mMin, mValue otherwise.
	template<typename T1, typename T2> inline constexpr	Common<T1, T2> getClampedMin(const T1& mValue, const T2& mMin) noexcept { return mValue < mMin ? mMin : mValue; }

	/// @brief Gets a clamped numeric value. (upper-bound only)
	/// @param mValue Const reference to the value. (original value won't be changed)
	/// @param mMax Upper bound.
	/// @return Returns mMax if mValue > mMax, mValue otherwise.
	template<typename T1, typename T2> inline constexpr Common<T1, T2> getClampedMax(const T1& mValue, const T2& mMax) noexcept { return mValue > mMax ? mMax : mValue; }

	/// @brief Gets a clamped numeric value.
	/// @param mValue Const reference to the value. (original value won't be changed)
	/// @param mMin Lower bound.
	/// @param mMax Upper bound.
	/// @return Returns mMax if mValue > mMax, mMin if mValue < mMin, mValue if mMin < mValue < mMax.
	template<typename T1, typename T2, typename T3> inline constexpr Common<T1, T2, T3> getClamped(const T1& mValue, const T2& mMin, const T3& mMax) noexcept
	{
		SSVU_ASSERT_CONSTEXPR(mMin <= mMax);
		return mValue < mMin ? mMin : (mValue > mMax ? mMax : mValue);
	}

	/// @brief Cycles a value in a range.
	/// @param mValue Const reference to the value. (original value won't be changed)
	/// @param mMin Range min.
	/// @param mMax Range max.
	/// @return Returns the cycled value.
	template<typename T1, typename T2, typename T3> inline auto getCycledValue(const T1& mValue, const T2& mMin, const T3& mMax)
	{
		Common<T1, T2, T3> delta{mMax - mMin}, result{std::fmod(mValue - mMin, delta)};
		if(result < 0) result += delta;
		return mMin + result;
	}

	/// @brief Converts degrees to radians.
	/// @param mValue Const reference to the value. (original value won't be changed)
	/// @return Returns the value in radians.
	template<typename T> inline constexpr T toRad(const T& mValue) noexcept { return mValue * radDegRatio; }

	/// @brief Converts radians to degrees.
	/// @param mValue Const reference to the value. (original value won't be changed)
	/// @return Returns the value in degrees.
	template<typename T> inline constexpr T toDeg(const T& mValue) noexcept { return mValue / radDegRatio; }

	/// @brief Restricts a radian value between 0 and 6.28f.
	/// @param mValue Const reference to the value. (original value won't be changed)
	/// @return Returns the restricted value in radians.
	template<typename T> inline T wrapRad(T mValue) noexcept { mValue = std::fmod(mValue, tau); return mValue < 0 ? mValue + tau : mValue; }

	/// @brief Restricts a degree value between 0 and 360.f.
	/// @param mValue Const reference to the value. (original value won't be changed)
	/// @return Returns the restricted value in degrees.
	template<typename T> inline T wrapDeg(T mValue) noexcept { mValue = std::fmod(mValue, 360.f); return mValue < 0 ? mValue + 360.f : mValue; }

	/// @brief Gets a rotated angle in degrees.
	/// @details Example use: a character slowly aiming towards the mouse position.
	/// @param mStart Angle to start from.
	/// @param mEnd Target angle.
	/// @param mSpeed Rotation speed.
	/// @return Returns the rotated angle in degrees.
	template<typename T1, typename T2, typename T3> inline auto getRotatedDeg(const T1& mStart, const T2& mEnd, const T3& mSpeed) noexcept
	{
		using CT = Common<T1, T2, T3>;
		CT diff{getCycledValue(wrapDeg(mEnd) - wrapDeg(mStart), -CT(180), CT(180))};
		if(diff < -mSpeed) return mStart - mSpeed;
		if(diff > mSpeed) return mStart + mSpeed;
		return mEnd;
	}

	/// @brief Gets a rotated angle in radians.
	/// @details Example use: a character slowly aiming towards the mouse position.
	/// @param mStart Angle to start from.
	/// @param mEnd Target angle.
	/// @param mSpeed Rotation speed.
	/// @return Returns the rotated angle in radians.
	template<typename T1, typename T2, typename T3> inline constexpr auto getRotatedRad(const T1& mStart, const T2& mEnd, const T3& mSpeed) noexcept
	{
		return getRotatedDeg(toDeg(mStart), toDeg(mEnd), mSpeed);
	}

	/// @brief Gets a 1D index from a 2D index.
	/// @details Useful when dealing with "implicit 2D" arrays, that are stored as 1D arrays.
	/// Internally, it calculates and returns `mY * mCols + mX`.
	/// @param mX X index.
	/// @param mY Y index.
	/// @param mCols Number of columns of the 2D array.
	/// @return Returns a 1D index for an "implicit 2D" array with `mCols` columns.
	template<typename T1, typename T2, typename T3> inline constexpr Common<T1, T2, T3> get1DIdxFrom2D(const T1& mX, const T2& mY, const T3& mCols) noexcept
	{
		SSVU_ASSERT_CONSTEXPR(mCols >= 0);
		return mX + mY * mCols;
	}

	/// @brief Gets a 1D index from a 3D index.
	/// @details Useful when dealing with "implicit 3D" arrays, that are stored as 1D arrays.
	/// @param mX X index.
	/// @param mY Y index.
	/// @param mZ Z index.
	/// @param mCols Number of columns of the 3D array.
	/// @param mRows Number of rows of the 3D array.
	/// @return Returns a 1D index for an "implicit 3D" array.
	template<typename T1, typename T2, typename T3, typename T4, typename T5>
		inline constexpr Common<T1, T2, T3, T4, T5> get1DIdxFrom3D(const T1& mX, const T2& mY, const T3& mZ, const T4& mCols, const T5& mRows) noexcept
	{
		SSVU_ASSERT_CONSTEXPR(mCols >= 0 && mRows >= 0);
		return mX + mY * mCols + mZ * mCols * mRows;
	}

	/// @brief Gets a 2D index from an 1D index.
	/// @details Useful when dealing with "implicit 2D" arrays, that are stored as 1D arrays.
	/// @param mIdx 1D index.
	/// @param mCols Number of columns of the 2D array.
	/// @return Returns a 2D index (under the form of an std::tuple) for a 2D array with `mCols` columns.
	template<typename T1, typename T2> inline auto get2DIdxFrom1D(const T1& mIdx, const T2& mCols) noexcept
	{
		SSVU_ASSERT(mIdx > 0 && mCols != 0);
		Common<T1, T2> y{mIdx / mCols};
		return std::make_tuple(mIdx - y * mCols, y);
	}

	/// @brief Gets a 3D index from an 1D index.
	/// @details Useful when dealing with "implicit 3D" arrays, that are stored as 1D arrays.
	/// @param mIdx 1D index.
	/// @param mCols Number of columns of the 2D array.
	/// @param mRows Number of rows of the 3D array.
	/// @return Returns a 3D index (under the form of an std::tuple) for an "implicit 3D" array.
	template<typename T1, typename T2, typename T3> inline auto get3DIdxFrom1D(const T1& mIdx, const T2& mCols, const T3& mRows) noexcept
	{
		SSVU_ASSERT(mIdx > 0 && mRows != 0 && mCols != 0);
		Common<T1, T2, T3> y{mIdx / mCols};
		return std::make_tuple(y, y % mRows, mIdx / (mCols * mRows));
	}

	/// @brief Gets mathematically correct modulo calculation.
	/// @details Assumes the modulo is positive.
	/// @code
	/// SSVU_ASSERT(getMod(-2, 12) == 10);
	/// @endcode
	/// @param mVal Left side of operation.
	/// @param mUB Right side of operation. Must be positive.
	/// @return Returns the mathematically correct mA % mB.
	template<typename T1, typename T2> inline Common<T1, T2> getMod(const T1& mVal, const T2& mUB)
	{
		SSVU_ASSERT(mUB > 0);
		auto result(((mVal % mUB) + mUB) % mUB);
		SSVU_ASSERT(result >= 0 && result < mUB);
		return result;
	}

	/// @brief Wraps a value between two other values.
	/// @details Assumes mUB - mLB is positive.
	/// @code
	/// SSVU_ASSERT(getMod(-2, 0, 12) == 9);
	/// @endcode
	/// @param mVal Index value to wrap.
	/// @param mLB Lower bound of possible indices (inclusive).
	/// @param mUB Upper bound of possible indices (exclusive).
	/// @return Returns the wrapped index value.
	template<typename T1, typename T2, typename T3> inline Common<T1, T2, T3> getMod(T1 mVal, const T2& mLB, const T3& mUB) noexcept
	{
		SSVU_ASSERT(mLB < mUB);

		const auto& rangeSize(mUB - mLB);
		SSVU_ASSERT(rangeSize > 0 && rangeSize + 1 != 0);

		if(mVal < mLB) mVal += rangeSize * ((mLB - mVal) / rangeSize + 1);
		return mLB + ((mVal - mLB) % rangeSize);
	}

	/// @brief Calculates Euclidean distance (squared) between two points.
	/// @param mX1 First point X.
	/// @param mY1 First point Y.
	/// @param mX2 Second point X.
	/// @param mY2 Second point Y.
	/// @return Returns the calculated distance (squared).
	template<typename T1, typename T2, typename T3, typename T4> inline constexpr Common<T1, T2, T3, T4> getDistSquaredEuclidean(const T1& mX1, const T2& mY1, const T3& mX2, const T4& mY2) noexcept
	{
		return (mX1 - mX2) * (mX1 - mX2) + (mY1 - mY2) * (mY1 - mY2);
	}

	/// @brief Calculates Euclidean distance between two points.
	/// @param mX1 First point X.
	/// @param mY1 First point Y.
	/// @param mX2 Second point X.
	/// @param mY2 Second point Y.
	/// @return Returns the calculated distance.
	template<typename T1, typename T2, typename T3, typename T4> inline constexpr Common<T1, T2, T3, T4> getDistEuclidean(const T1& mX1, const T2& mY1, const T3& mX2, const T4& mY2) noexcept
	{
		return std::sqrt(getDistSquaredEuclidean(mX1, mY1, mX2, mY2));
	}

	/// @brief Calculates Manhattan distance between two points.
	/// @param mX1 First point X.
	/// @param mY1 First point Y.
	/// @param mX2 Second point X.
	/// @param mY2 Second point Y.
	/// @return Returns the calculated distance.
	template<typename T1, typename T2, typename T3, typename T4> inline constexpr Common<T1, T2, T3, T4> getDistManhattan(const T1& mX1, const T2& mY1, const T3& mX2, const T4& mY2) noexcept
	{
		return std::abs(mX1 - mX2) + std::abs(mY1 - mY2);
	}

	/// @brief Calculates Chebyshev distance between two points.
	/// @param mX1 First point X.
	/// @param mY1 First point Y.
	/// @param mX2 Second point X.
	/// @param mY2 Second point Y.
	/// @return Returns the calculated distance.
	template<typename T1, typename T2, typename T3, typename T4> inline constexpr Common<T1, T2, T3, T4> getDistChebyshev(const T1& mX1, const T2& mY1, const T3& mX2, const T4& mY2) noexcept
	{
		return std::max(std::abs(mX2 - mX1), std::abs(mY2 - mY1));
	}

	/// @brief Calculates degrees needed to turn towards a point.
	/// @param mX1 First point X.
	/// @param mY1 First point Y.
	/// @param mX2 Second point X.
	/// @param mY2 Second point Y.
	/// @return Returns the needed degrees.
	template<typename T1, typename T2, typename T3, typename T4> inline constexpr Common<T1, T2, T3, T4> getRadTowards(const T1& mX1, const T2& mY1, const T3& mX2, const T4& mY2) noexcept
	{
		return std::atan2(mY2 - mY1, mX2 - mX1);
	}

	/// @brief Calculates radians needed to turn towards a point.
	/// @param mX1 First point X.
	/// @param mY1 First point Y.
	/// @param mX2 Second point X.
	/// @param mY2 Second point Y.
	/// @return Returns the needed radians.
	template<typename T1, typename T2, typename T3, typename T4> inline constexpr Common<T1, T2, T3, T4> getDegTowards(const T1& mX1, const T2& mY1, const T3& mX2, const T4& mY2) noexcept
	{
		return toDeg(getRadTowards(mX1, mY1, mX2, mY2));
	}

	/// @brief Gets the smallest distance between two angles. (radians)
	/// @param mA First angle.
	/// @param mB Second angle.
	/// @return Returns the smallest distance in radians between mA and mB.
	template<typename T1, typename T2> inline constexpr Common<T1, T2> getDistRad(const T1& mA, const T2& mB) noexcept { return pi - std::abs(std::fmod(std::abs(mA - mB), pi * 2.f) - pi); }

	/// @brief Gets the smallest distance between two angles. (degrees)
	/// @param mA First angle.
	/// @param mB Second angle.
	/// @return Returns the smallest distance in degrees between mA and mB.
	template<typename T1, typename T2> inline constexpr Common<T1, T2> getDistDeg(const T1& mA, const T2& mB) noexcept { return 180.f - std::abs(std::fmod(std::abs(mA - mB), 360.f) - 180.f); }

	/// @brief Maps a numeric value from an input range to an output range.
	/// @param mI Input value.
	/// @param mIMin Input range min.
	/// @param mIMax Input range max.
	/// @param mOMin Output range min.
	/// @param mOMax Output range max.
	template<typename T1, typename T2, typename T3, typename T4, typename T5>
		inline constexpr Common<T1, T2, T3, T4, T5> getMap(const T1& mI, const T2& mIMin, const T3& mIMax, const T4& mOMin, const T5& mOMax) noexcept
	{
		SSVU_ASSERT_CONSTEXPR((mIMax - mIMin) != 0);
		return (mI - mIMin) * (mOMax - mOMin) / (mIMax - mIMin) + mOMin;
	}

	/// @brief Lineally interpolates between two values using a [0..1] weight.
	/// @param mWeight Weight between 0 and 1.
	/// @param mA Min value,
	/// @param mB Max value.
	template<typename T1, typename T2, typename T3> inline constexpr Common<T1, T2, T3> getLerp(const T1& mWeight, const T2& mA, const T3& mB) noexcept
	{
		SSVU_ASSERT_CONSTEXPR(mWeight >= 0.f && mWeight <= 1.f);
		return (mA + mWeight * (mB - mA));
	}
}

#endif
