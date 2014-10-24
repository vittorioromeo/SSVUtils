// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_INTERNAL_SHAREDFUNCS
#define SSVU_INTERNAL_SHAREDFUNCS

namespace ssvu
{
	namespace Internal
	{
		template<typename TF1, typename TF2, typename TF3> inline void refreshImpl(SizeT& mSize, SizeT& mSizeNext, const TF1& mFAliveChk, const TF2& mFSwap, const TF3& mFDeinit) noexcept
		{
			const int intSizeNext(mSizeNext);
			int iD{0}, iA{intSizeNext - 1};

			do
			{
				// Find dead item from left
				for(; true; ++iD)
				{
					// No more dead items
					if(iD > iA) goto finishRefresh;
					if(!mFAliveChk(iD)) break;
				}

				// Find alive item from right
				for(; true; --iA)
				{
					// No more alive items
					if(iA <= iD) goto finishRefresh;
					if(mFAliveChk(iA)) break;
				}

				SSVU_ASSERT(!mFAliveChk(iD) && mFAliveChk(iA));
				mFSwap(iD, iA);
				SSVU_ASSERT(mFAliveChk(iD) && !mFAliveChk(iA));

				// Move both iterators
				++iD; --iA;
			}
			while(true);

			finishRefresh:

			#if SSVU_DEBUG
				for(iA = iA - 1; iA >= 0; --iA) SSVU_ASSERT(mFAliveChk(iA));
			#endif

			mSize = mSizeNext = iD;

			for(; iD < intSizeNext; ++iD)
			{
				SSVU_ASSERT(!mFAliveChk(iD));
				mFDeinit(iD);
			}
		}
	}
}

#endif
