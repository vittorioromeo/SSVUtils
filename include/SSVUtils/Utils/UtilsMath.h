// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_UTILS_MATH
#define SSVU_UTILS_MATH

#include <cassert>
#include <array>
#include <random>
#include "SSVUtils/Global/Typedefs.h"

namespace ssvu
{
	static std::minstd_rand rndEngine;
	template<typename T> using RndDistributionI = std::uniform_int_distribution<T>;
	template<typename T> using RndDistributionR = std::uniform_real_distribution<T>;

	constexpr float pi{3.14159265f};
	constexpr float degRadRatio{pi / 180.f}; // C++14: templatized values

	/// @brief Gets a random integer value between [mMin and mMax).
	/// @tparam T Type of integer value. (default int)
	/// @param mMin Lower inclusive bound.
	/// @param mMax Upper exclusive bound.
	/// @return Returns a random integer value, between [mMin and mMax).
	template<typename T = int> inline T getRnd(const T& mMin, const T& mMax) { assert(mMin < mMax); return RndDistributionI<T>{mMin, mMax - 1}(rndEngine); }

	/// @brief Gets a random real value between [mMin and mMax).
	/// @tparam T Type of real value. (default float)
	/// @param mMin Lower inclusive bound.
	/// @param mMax Upper exclusive bound.
	/// @return Returns a random real value, between [mMin and mMax).
	template<typename T = float> inline T getRndR(const T& mMin, const T& mMax) { assert(mMin < mMax); return RndDistributionR<T>{mMin, mMax - 1}(rndEngine); }

	/// @brief Gets a random sign.
	/// @tparam T Type of integer value. (default int)
	/// @return Returns -1 or 1.
	template<typename T = int> inline T getRndSign() { return RndDistributionI<T>{0, 1}(rndEngine) > 0 ? -1 : 1; }

	/// @brief Gets the sign of a numeric value. (unsigned version)
	/// @param mValue Value to use.
	/// @return Returns 1 if the value is >0, -1 if the value is < 0, 0 if the value == 0.
	template<typename T> constexpr inline int getSign(const T& mValue, EnableIf<!std::is_signed<T>::value>* = nullptr) noexcept { return 0 < mValue; }

	/// @brief Gets the sign of a numeric value. (signed version)
	/// @param mValue Value to use.
	/// @return Returns 1 if the value is >0, -1 if the value is < 0, 0 if the value == 0.
	template<typename T> constexpr inline int getSign(const T& mValue, EnableIf<std::is_signed<T>::value>* = nullptr) noexcept { return (0 < mValue) - (mValue < 0); }

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
	template<typename T1, typename T2, typename T3> inline void clamp(T1& mValue, const T2& mMin, const T3& mMax) noexcept { assert(mMin <= mMax); if(mValue < mMin) mValue = mMin; else if(mValue > mMax) mValue = mMax; }

	/// @brief Gets a clamped numeric value. (lower-bound only)
	/// @param mValue Const reference to the value. (original value won't be changed)
	/// @param mMin Lower bound.
	/// @return Returns mMin if mValue < mMin, mValue otherwise.
	template<typename T1, typename T2> constexpr inline	Common<T1, T2> getClampedMin(const T1& mValue, const T2& mMin) noexcept { return mValue < mMin ? mMin : mValue; }

	/// @brief Gets a clamped numeric value. (upper-bound only)
	/// @param mValue Const reference to the value. (original value won't be changed)
	/// @param mMax Upper bound.
	/// @return Returns mMax if mValue > mMax, mValue otherwise.
	template<typename T1, typename T2> constexpr inline Common<T1, T2> getClampedMax(const T1& mValue, const T2& mMax) noexcept { return mValue > mMax ? mMax : mValue; }

	/// @brief Gets a clamped numeric value.
	/// @param mValue Const reference to the value. (original value won't be changed)
	/// @param mMin Lower bound.
	/// @param mMax Upper bound.
	/// @return Returns mMax if mValue > mMax, mMin if mValue < mMin, mValue if mMin < mValue < mMax.
	template<typename T1, typename T2, typename T3> constexpr inline Common<T1, T2, T3> getClamped(const T1& mValue, const T2& mMin, const T3& mMax) noexcept { /*assert(mMin <= mMax);*/ return mValue < mMin ? mMin : (mValue > mMax ? mMax : mValue); }

	/// @brief Cycles a value in a range.
	/// @param mValue Const reference to the value. (original value won't be changed)
	/// @param mMin Range min.
	/// @param mMax Range max.
	/// @return Returns the cycled value.
	template<typename T1, typename T2, typename T3> inline Common<T1, T2, T3> getCycledValue(const T1& mValue, const T2& mMin, const T3& mMax)
	{
		using CT = Common<T1, T2, T3>;
		CT delta{mMax - mMin}, result{std::fmod(mValue - mMin, delta)};
		if(result < 0) result += delta;
		return mMin + result;
	}

	/// @brief Converts degrees to radians.
	/// @param mValue Const reference to the value. (original value won't be changed)
	/// @return Returns the value in radians.
	template<typename T> constexpr inline T toRad(const T& mValue) noexcept { return mValue * degRadRatio; }

	/// @brief Converts radians to degrees.
	/// @param mValue Const reference to the value. (original value won't be changed)
	/// @return Returns the value in degrees.
	template<typename T> constexpr inline T toDeg(const T& mValue) noexcept { return mValue / degRadRatio; }

	/// @brief Restricts a radian value between 0 and 6.28f.
	/// @param mValue Const reference to the value. (original value won't be changed)
	/// @return Returns the restricted value in radians.
	template<typename T> inline T wrapRad(T mValue) noexcept { mValue = std::fmod(mValue, 6.28f); return mValue < 0 ? mValue + 6.28f : mValue; }

	/// @brief Restricts a degree value between 0 and 360.f.
	/// @param mValue Const reference to the value. (original value won't be changed)
	/// @return Returns the restricted value in degrees.
	template<typename T> inline T wrapDeg(T mValue) noexcept { mValue = std::fmod(mValue, 360.f); return mValue < 0 ? mValue + 360.f : mValue; }

	/// @brief Gets a rotated angle in degrees.
	/// @details Example use: a character slowly aiming towards the mouse position.
	/// @tparam J Type of rotation value.
	/// @param mStart Angle to start from.
	/// @param mEnd Target angle.
	/// @param mSpeed Rotation speed.
	/// @return Returns the rotated angle in degrees.
	template<typename T1, typename T2, typename T3> inline Common<T1, T2, T3> getRotatedDeg(const T1& mStart, const T2& mEnd, const T3& mSpeed) noexcept
	{
		using CT = Common<T1, T2, T3>;
		CT diff{getCycledValue(wrapDeg(mEnd) - wrapDeg(mStart), -CT(180), CT(180))};
		if(diff < -mSpeed) return mStart - mSpeed;
		if(diff > mSpeed) return mStart + mSpeed;
		return mEnd;
	}

	/// @brief Gets a rotated angle in radians.
	/// @details Example use: a character slowly aiming towards the mouse position.
	/// @tparam J Type of rotation value.
	/// @param mStart Angle to start from.
	/// @param mEnd Target angle.
	/// @param mSpeed Rotation speed.
	/// @return Returns the rotated angle in radians.
	template<typename T1, typename T2, typename T3> inline Common<T1, T2, T3> getRotatedRad(const T1& mStart, const T2& mEnd, const T3& mSpeed) noexcept { return getRotatedDeg(toDeg(mStart), toDeg(mEnd), mSpeed); }

	/// @brief Gets a 1D index from a 2D index.
	/// @details Useful when dealing with "implicit 2D" arrays, that are stored as 1D arrays.
	/// Internally, it calculates and returns `mY * mColumns + mX`.
	/// @tparam T Type of index value.
	/// @param mX X index.
	/// @param mY Y index.
	/// @param mColumns Number of columns of the 2D array.
	/// @return Returns a 1D index for an "implicit 2D" array with `mColumns` columns.
	template<typename T> constexpr inline T get1DIdxFrom2D(const T& mX, const T& mY, const T& mColumns) noexcept { return mX + mY * mColumns; }

	/// @brief Gets a 1D index from a 3D index.
	/// @details Useful when dealing with "implicit 3D" arrays, that are stored as 1D arrays.
	/// @tparam T Type of index value.
	/// @param mX X index.
	/// @param mY Y index.
	/// @param mZ Z index.
	/// @param mColumns Number of columns of the 3D array.
	/// @param mRows Number of rows of the 3D array.
	/// @return Returns a 1D index for an "implicit 3D" array.
	template<typename T> constexpr inline T get1DIdxFrom3D(const T& mX, const T& mY, const T& mZ, const T& mColumns, const T& mRows) noexcept { return mX + mY * mColumns + mZ * mColumns * mRows; }

	/// @brief Gets a 2D index from an 1D index.
	/// @details Useful when dealing with "implicit 2D" arrays, that are stored as 1D arrays.
	/// @tparam T Type of index value.
	/// @param mIdx 1D index.
	/// @param mColumns Number of columns of the 2D array.
	/// @return Returns a 2D index (under the form of an std::array<T, 2>) for a 2D array with `mColumns` columns.
	template<typename T> inline std::array<T, 2> get2DIdxFrom1D(const T& mIdx, const T& mColumns) noexcept { assert(mIdx > 0); T y{mIdx / mColumns}; return {{mIdx - y * mColumns, y}}; }

	/// @brief Gets a 3D index from an 1D index.
	/// @details Useful when dealing with "implicit 3D" arrays, that are stored as 1D arrays.
	/// @tparam T Type of index value.
	/// @param mIdx 1D index.
	/// @param mColumns Number of columns of the 2D array.
	/// @param mRows Number of rows of the 3D array.
	/// @return Returns a 3D index (under the form of an std::array<T, 3>) for an "implicit 3D" array.
	template<typename T> inline std::array<T, 3> get3DIdxFrom1D(const T& mIdx, const T& mColumns, const T& mRows) noexcept { assert(mIdx > 0); return {{mIdx / mColumns, (mIdx / mColumns) % mRows, mIdx / (mColumns * mRows)}}; }

	/// @brief Gets sign-indepedent modulo calculation.
	/// @code
	/// assert(getSIMod(-2, 12) == 10);
	/// assert(getSIMod(2, -12) == -10);
	/// @endcode
	/// @param mA Left side of operation.
	/// @param mB Right side of operation.
	/// @return Returns the mathematically correct mA % mB.
	template<typename T1, typename T2> inline Common<T1, T2> getSIMod(const T1& mA, const T2& mB)
	{
		assert(mB != 0);
		if(mB < 0) return getSIMod(-mA, -mB);
		auto result(mA % mB);
		if(result < 0) result += mB;
		return result;
	}

	/// @brief Gets a wrapped index value.
	/// @code
	/// assert(getWrapIdx(-2, 0, 12) == 9);
	/// @endcode
	/// @param mVal Index value to wrap.
	/// @param mLowerBound Lower bound of possible indices (inclusive).
	/// @param mUpperBound Upper bound of possible indices (exclusive).
	/// @return Returns the wrapped index value.
	template<typename T1, typename T2, typename T3> inline Common<T1, T2, T3> getWrapIdx(T1 mVal, const T2& mLowerBound, const T3& mUpperBound) noexcept
	{
		assert(mLowerBound < mUpperBound);
		const auto& rangeSize(mUpperBound - 1 - mLowerBound + 1);
		if(mVal < mLowerBound) mVal += rangeSize * ((mLowerBound - mVal) / rangeSize + 1);
		return mLowerBound + (mVal - mLowerBound) % rangeSize;
	}

	/// @brief Gets a wrapped index value. (Default inclusive lower bound: 0)
	/// @code
	/// assert(getWrapIdx(-2, 12) == 9);
	/// @endcode
	/// @param mVal Index value to wrap.
	/// @param mUpperBound Upper bound of possible indices (exclusive).
	/// @return Returns the wrapped index value.
	template<typename T1, typename T2> inline Common<T1, T2> getWrapIdx(const T1& mVal, const T2& mUpperBound) noexcept { return getWrapIdx(mVal, T2(0), mUpperBound); }

	/// @brief Calculates Euclidean distance (squared) between two points.
	/// @param mX1 First point X.
	/// @param mY1 First point Y.
	/// @param mX2 Second point X.
	/// @param mY2 Second point Y.
	/// @return Returns the calculated distance (squared).
	template<typename T1, typename T2, typename T3, typename T4> inline Common<T1, T2, T3, T4> getDistSquaredEuclidean(const T1& mX1, const T2& mY1, const T3& mX2, const T4& mY2) noexcept { return (mX1 - mX2) * (mX1 - mX2) + (mY1 - mY2) * (mY1 - mY2); }

	/// @brief Calculates Euclidean distance between two points.
	/// @param mX1 First point X.
	/// @param mY1 First point Y.
	/// @param mX2 Second point X.
	/// @param mY2 Second point Y.
	/// @return Returns the calculated distance.
	template<typename T1, typename T2, typename T3, typename T4> inline Common<T1, T2, T3, T4> getDistEuclidean(const T1& mX1, const T2& mY1, const T3& mX2, const T4& mY2) noexcept { return std::sqrt(getDistSquaredEuclidean(mX1, mY1, mX2, mY2)); }

	/// @brief Calculates Manhattan distance between two points.
	/// @param mX1 First point X.
	/// @param mY1 First point Y.
	/// @param mX2 Second point X.
	/// @param mY2 Second point Y.
	/// @return Returns the calculated distance.
	template<typename T1, typename T2, typename T3, typename T4> inline Common<T1, T2, T3, T4> getDistManhattan(const T1& mX1, const T2& mY1, const T3& mX2, const T4& mY2) noexcept { return std::abs(mX1 - mX2) + std::abs(mY1 - mY2); }

	/// @brief Calculates Chebyshev distance between two points.
	/// @param mX1 First point X.
	/// @param mY1 First point Y.
	/// @param mX2 Second point X.
	/// @param mY2 Second point Y.
	/// @return Returns the calculated distance.
	template<typename T1, typename T2, typename T3, typename T4> inline Common<T1, T2, T3, T4> getDistChebyshev(const T1& mX1, const T2& mY1, const T3& mX2, const T4& mY2) noexcept { return std::max(std::abs(mX2 - mX1), std::abs(mY2 - mY1)); }

	/// @brief Calculates degrees needed to turn towards a point.
	/// @param mX1 First point X.
	/// @param mY1 First point Y.
	/// @param mX2 Second point X.
	/// @param mY2 Second point Y.
	/// @return Returns the needed degrees.
	template<typename T1, typename T2, typename T3, typename T4> inline Common<T1, T2, T3, T4> getRadTowards(const T1& mX1, const T2& mY1, const T3& mX2, const T4& mY2) noexcept { return std::atan2(mY2 - mY1, mX2 - mX1); }

	/// @brief Calculates radians needed to turn towards a point.
	/// @param mX1 First point X.
	/// @param mY1 First point Y.
	/// @param mX2 Second point X.
	/// @param mY2 Second point Y.
	/// @return Returns the needed radians.
	template<typename T1, typename T2, typename T3, typename T4> inline Common<T1, T2, T3, T4> getDegTowards(const T1& mX1, const T2& mY1, const T3& mX2, const T4& mY2) noexcept { return toDeg(getRadTowards(mX1, mY1, mX2, mY2)); }
}

#endif

