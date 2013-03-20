// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_UTILS_MATH
#define SSVU_UTILS_MATH

namespace ssvu
{
	template<typename T> constexpr int getSign(const T& mValue) { return mValue > 0 ? 1 : -1; }
	template<typename T> constexpr T clamp(const T& mValue, const T& mLow, const T& mHigh) { return mValue < mLow ? mLow : (mValue > mHigh ? mHigh : mValue); }
	template<typename T> constexpr T toRadians(const T& mValue) { return mValue / 57.3f; }
	template<typename T> constexpr T toDegrees(const T& mValue) { return mValue * 57.3f; }
	template<typename T> T wrapRadians(const T& mValue) { T result(fmod(mValue, 6.28f)); if(result < 0) result += 6.28f; return result; }
	template<typename T> T wrapDegrees(const T& mValue) { T result(fmod(mValue, 360.f)); if(result < 0) result += 360.f; return result; }
	template<typename T, typename J> T getRotatedRadians(const T& mStart, const T& mEnd, const J& mSpeed)
	{
		T start(wrapRadians(mStart)), end(wrapRadians(mEnd));
		if(abs(start - end) < mSpeed) return end;
		return wrapRadians(start + (sin(end - start)) * mSpeed);
	}
	template<typename T, typename J> T getRotatedDegrees(const T& mStart, const T& mEnd, const J& mSpeed)
	{
		T start(wrapDegrees(mStart)), end(wrapDegrees(mEnd));
		if(abs(start - end) < mSpeed) return end;
		return wrapDegrees(start + (sin((end - start) / 57.3f)) *mSpeed);
	}

	int getRnd(int mMin, int mMax);
	float getSaturated(float mValue);
	float getSmootherStep(float, float, float);
}

#endif
