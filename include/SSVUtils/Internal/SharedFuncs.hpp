// Copyright (c) 2013-2015 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_INTERNAL_SHAREDFUNCS
#define SSVU_INTERNAL_SHAREDFUNCS

namespace ssvu
{
	namespace Impl
	{
		template<typename TF1, typename TF2, typename TF3>
		inline auto refreshImplLoop(SizeT& mSizeNext, const TF1& mFAliveChk, const TF2& mFSwap, const TF3& mFDeinit) noexcept
		{
			SizeT iD{0}, iA{mSizeNext - 1};

			while(true)
			{
				// Find dead item from left
				for(; true; ++iD)
				{
					// Order matters.
					if(SSVU_UNLIKELY(iD > iA))
					{
						// No more dead items
						return iD;
					}

					if(SSVU_UNLIKELY(!mFAliveChk(iD))) break;
				}

				// Find alive item from right
				for(; true; --iA)
				{
					// Order matters.
					if(SSVU_UNLIKELY(mFAliveChk(iA))) break;
					mFDeinit(iA);

					if(SSVU_UNLIKELY(iA <= iD))
					{
						// No more alive items
						return iD;
					}
				}

				SSVU_ASSERT(!mFAliveChk(iD) && mFAliveChk(iA));
				mFSwap(iD, iA);
				mFDeinit(iA);

				// Move both iterators
				++iD; --iA;
			}

			return iD;
		}

		template<typename TF1, typename TF2, typename TF3>
		inline void refreshImpl(SizeT& mSize, SizeT& mSizeNext, const TF1& mFAliveChk, const TF2& mFSwap, const TF3& mFDeinit) noexcept
		{
			if(SSVU_UNLIKELY(mSizeNext == 0))
			{
				mSize = 0;
				return;
			}

			auto iD(refreshImplLoop(mSizeNext, mFAliveChk, mFSwap, mFDeinit));
			mSize = mSizeNext = iD;

			#if defined(SSVU_DEBUG)
				for(auto i(0u); i < iD; ++i)
				{
					SSVU_ASSERT(mFAliveChk(i));
				}
			#endif
		}
	}
}

#endif
