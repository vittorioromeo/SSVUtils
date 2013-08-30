// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_TICKER
#define SSVU_TICKER

namespace ssvu
{
	template<typename T> constexpr inline static T framesToSecs(T mFrames)	{ return mFrames / 60.f; }
	template<typename T> constexpr inline static T secsToFrames(T mSecs)	{ return mSecs * 60.f; }

	class Ticker
	{
		private:
			bool enabled{true};
			float current{0.f}, total{0.f}, target;
			unsigned int ticks{0};

			inline void setEnabled(bool mEnabled)	{ enabled = mEnabled; }
			inline void setTarget(float mTarget)	{ target = mTarget; }
			inline bool hasReachedTarget() const	{ return current >= target; }

		public:
			Ticker(float mTarget, bool mEnabled = true) : target{mTarget}, enabled{mEnabled} { }

			inline bool update(float mFrameTime)
			{
				if(!enabled) return false;

				current += mFrameTime;
				total += mFrameTime;

				if(!hasReachedTarget()) return false;

				resetCurrent(); ++ticks;
				return true;
			}
			inline bool update(float mFrameTime, float mTarget) { setTarget(mTarget); return update(mFrameTime); }

			inline void resume()					{ setEnabled(true); }
			inline void restart()					{ resetCurrent(); setEnabled(true); }
			inline void restart(float mTarget)		{ resetCurrent(); setTarget(mTarget); setEnabled(true); }
			inline void pause()						{ setEnabled(false); }
			inline void stop()						{ resetCurrent(); setEnabled(false); }

			inline void resetCurrent()				{ current = 0.f; }
			inline void resetTicks()				{ ticks = 0; }
			inline void resetTotal()				{ total = 0.f; }
			inline void resetAll()					{ resetCurrent(); resetTicks(); resetTotal(); }

			inline bool isEnabled() const			{ return enabled; }
			inline float getTarget() const			{ return target; }
			inline float getCurrent() const			{ return current; }
			inline float getTotal() const			{ return total; }
			inline unsigned int getTicks() const	{ return ticks; }

			template<typename T = float> inline T getTotalSecs() const		{ return static_cast<T>(framesToSecs(total)); }
			template<typename T = float> inline T getCurrentSecs() const	{ return static_cast<T>(framesToSecs(current)); }
	};
}

#endif
