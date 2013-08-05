// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_UTILS_MATH
#define SSVU_UTILS_MATH

#include <array>
#include <random>

namespace ssvu
{
	static std::minstd_rand rndEngine;
	template<typename T> using RndDistributionI = std::uniform_int_distribution<T>;
	template<typename T> using RndDistributionF = std::uniform_real_distribution<T>;

	/*!
	 *
	 * @brief Gets a random integer value between mMin and mMax.
	 *
	 * @param mMin Lower bound.
	 * @param mMax Upper bound.
	 *
	 * @return Returns a random integer value, between mMin and mMax.
	 *
	 */
	inline int getRnd(int mMin, int mMax) { return RndDistributionI<int>{mMin, mMax - 1}(rndEngine); }

	/*!
	 *
	 * @brief Gets a random floating point value between mMin and mMax.
	 *
	 * @param mMin Lower bound.
	 * @param mMax Upper bound.
	 *
	 * @return Returns a random integer value, between mMin and mMax.
	 *
	 */
	inline float getRndF(float mMin, float mMax) { return RndDistributionF<float>{mMin, mMax - 1}(rndEngine); }

	/*!
	 *
	 * @brief Gets the sign of a numeric value.
	 *
	 * @tparam T Type of value.
	 * @param mValue Value to use.
	 *
	 * @return Returns 1 if the value is >0, -1 if the value is <= 0.
	 *
	 */
	template<typename T> constexpr inline int getSign(const T& mValue) { return mValue > 0 ? 1 : -1; }

	/*!
	 *
	 * @brief Gets a clamped numeric value.
	 *
	 * @tparam T Type of value.
	 * @param mValue Const reference to the value. (original value won't be changed)
	 * @param mMin Lower bound.
	 * @param mMax Upper bound.
	 *
	 * @return Returns mMax if mValue > mMax, mMin if mValue < mMin, mValue if mMin < mValue < mMax.
	 *
	 */
	template<typename T> constexpr inline T getClamped(const T& mValue, const T& mMin, const T& mMax) { return mValue < mMin ? mMin : (mValue > mMax ? mMax : mValue); }

	/*!
	 *
	 * @brief Converts degrees to radians.
	 *
	 * @tparam T Type of value.
	 * @param mValue Const reference to the value. (original value won't be changed)
	 *
	 * @return Returns the value in radians.
	 *
	 */
	template<typename T> constexpr inline T toRadians(const T& mValue) { return mValue / 57.3f; }

	/*!
	 *
	 * @brief Converts radians to degrees.
	 *
	 * @tparam T Type of value.
	 * @param mValue Const reference to the value. (original value won't be changed)
	 *
	 * @return Returns the value in degrees.
	 *
	 */
	template<typename T> constexpr inline T toDegrees(const T& mValue) { return mValue * 57.3f; }

	/*!
	 *
	 * @brief Restricts a radian value between 0 and 6.28f.
	 *
	 * @tparam T Type of value.
	 * @param mValue Const reference to the value. (original value won't be changed)
	 *
	 * @return Returns the restricted value in radians.
	 *
	 */
	template<typename T> inline T wrapRadians(T mValue) { mValue = fmod(mValue, 6.28f); if(mValue < 0) mValue += 6.28f; return mValue; }

	/*!
	 *
	 * @brief Restricts a degree value between 0 and 360.f.
	 *
	 * @tparam T Type of value.
	 * @param mValue Const reference to the value. (original value won't be changed)
	 *
	 * @return Returns the restricted value in degrees.
	 *
	 */
	template<typename T> inline T wrapDegrees(T mValue) { mValue = fmod(mValue, 360.f); if(mValue < 0) mValue += 360.f; return mValue; }

	/*!
	 *
	 * @brief Gets a rotated angle in radians.
	 *
	 * Example use: a character slowly aiming towards the mouse position.
	 *
	 * @tparam T Type of value.
	 * @tparam J Type of rotation value.
	 * @param mStart Angle to start from.
	 * @param mEnd Target angle.
	 * @param mSpeed Rotation speed.
	 *
	 * @return Returns the rotated angle in radians.
	 *
	 */
	template<typename T, typename J> inline T getRotatedRadians(const T& mStart, const T& mEnd, const J& mSpeed)
	{
		T start(wrapRadians(mStart)), end(wrapRadians(mEnd));
		if(abs(start - end) < mSpeed) return end;
		return wrapRadians(start + (sin(end - start)) * mSpeed);
	}

	/*!
	 *
	 * @brief Gets a rotated angle in degrees.
	 *
	 * Example use: a character slowly aiming towards the mouse position.
	 *
	 * @tparam T Type of value.
	 * @tparam J Type of rotation value.
	 * @param mStart Angle to start from.
	 * @param mEnd Target angle.
	 * @param mSpeed Rotation speed.
	 *
	 * @return Returns the rotated angle in degrees.
	 *
	 */
	template<typename T, typename J> inline T getRotatedDegrees(const T& mStart, const T& mEnd, const J& mSpeed)
	{
		T start(wrapDegrees(mStart)), end(wrapDegrees(mEnd));
		if(abs(start - end) < mSpeed) return end;
		return wrapDegrees(start + (sin((end - start) / 57.3f)) *mSpeed);
	}

	/*!
	 *
	 * @brief Gets a 1D index from a 2D index.
	 *
	 * Useful when dealing with "implicit 2D" arrays, that are stored as 1D arrays.
	 * Internally, it calculates and returns `mY * mColumns + mX`.
	 *
	 * @tparam T Type of index value.
	 * @param mX X index.
	 * @param mY Y index.
	 * @param mColumns Number of columns of the 2D array.
	 *
	 * @return Returns a 1D index for an "implicit 2D" array with `mColumns` columns.
	 *
	 */
	template<typename T> constexpr inline T get1DIndexFrom2D(const T& mX, const T& mY, const T& mColumns) { return mY * mColumns + mX; }

	/*!
	 *
	 * @brief Gets a 2D index from an 1D index.
	 *
	 * Useful when dealing with "implicit 2D" arrays, that are stored as 1D arrays.
	 *
	 * @tparam T Type of index value.
	 * @param mIndex 1D index.
	 * @param mColumns Number of columns of the 2D array.
	 *
	 * @return Returns a 2D index (under the form of an std::array<T, 2>) for a 2D array with `mColumns` columns.
	 *
	 */
	template<typename T> inline std::array<T, 2> get2DIndexFrom1D(const T& mIndex, const T& mColumns) { T y{mIndex / mColumns}; return std::array<T, 2>{{mIndex - y * mColumns, y}}; }
}

#endif

