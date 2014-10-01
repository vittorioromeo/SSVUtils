// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_JSON_NUM_NUMHELPER
#define SSVU_JSON_NUM_NUMHELPER

namespace ssvu
{
	namespace Json
	{
		namespace Internal
		{
			#define SSVU_JSON_DEFINE_NUMHELPER(mType, mRepresentation) \
				template<> struct NumHelper<mType> \
				{ \
					inline static void set(Num& mN, const mType& mX) noexcept { return SSVPP_CAT(mN.set, mRepresentation)(mX); } \
					inline static auto as(const Num& mN) noexcept { return static_cast<mType>(SSVPP_CAT(mN.get, mRepresentation)()); } \
				};

			SSVU_JSON_DEFINE_NUMHELPER(char, IntS)
			SSVU_JSON_DEFINE_NUMHELPER(int, IntS)
			SSVU_JSON_DEFINE_NUMHELPER(long int, IntS)

			SSVU_JSON_DEFINE_NUMHELPER(unsigned char, IntU)
			SSVU_JSON_DEFINE_NUMHELPER(unsigned int, IntU)
			SSVU_JSON_DEFINE_NUMHELPER(unsigned long int, IntU)

			SSVU_JSON_DEFINE_NUMHELPER(float, Real)
			SSVU_JSON_DEFINE_NUMHELPER(double, Real)

			#undef SSVU_JSON_DEFINE_NUMHELPER
		}
	}
}

#endif
