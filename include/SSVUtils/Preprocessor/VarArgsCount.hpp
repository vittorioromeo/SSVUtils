// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_PREPROCESSOR_VARARGSCOUNT
#define SSVU_PREPROCESSOR_VARARGSCOUNT

#include "SSVUtils/Preprocessor/Utils.hpp"

namespace ssvu
{
	#define SSVPP_IMPL_ARG_40(	m00, m01, m02, m03, m04, m05, m06, m07, m08, m09, m10, m11, m12, m13, m14, m15, m16, m17, m18, m19, m20, \
								m21, m22, m23, m24, m25, m26, m27, m28, m29, m30, m31, m32, m33, m34, m35, m36, m37, m38, m39, ...) m39

	#define SSVPP_IMPL_HAS_COMMA_40(...)	SSVPP_IMPL_ARG_40(__VA_ARGS__, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0)
	#define SSVPP_IMPL_HAS_NO_COMMA_40(...)	SSVPP_IMPL_ARG_40(__VA_ARGS__, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1)

	#define SSVPP_IMPL_TRIGGER_PARENTHESIS(...) ,

	#define SSVPP_IMPL_HAS_ZERO_OR_ONE_ARGS_IMPL(m0, m1, m2, m3) SSVPP_IMPL_HAS_NO_COMMA_40(SSVPP_TKNCAT_5(SSVPP_IMPL_IS_EMPTY_CASE_, m0, m1, m2, m3))
	#define SSVPP_IMPL_HAS_ZERO_OR_ONE_ARGS(...)	SSVPP_IMPL_HAS_ZERO_OR_ONE_ARGS_IMPL(SSVPP_IMPL_HAS_COMMA_40(__VA_ARGS__), \
													SSVPP_IMPL_HAS_COMMA_40(SSVPP_IMPL_TRIGGER_PARENTHESIS __VA_ARGS__), \
													SSVPP_IMPL_HAS_COMMA_40(__VA_ARGS__ (~)), \
													SSVPP_IMPL_HAS_COMMA_40(SSVPP_IMPL_TRIGGER_PARENTHESIS __VA_ARGS__ (~)))

	#define SSVPP_IMPL_IS_EMPTY_CASE_0001 ,

	#define SSVPP_IMPL_VA_NUM_ARGS_0(...) SSVPP_IMPL_HAS_ZERO_OR_ONE_ARGS(__VA_ARGS__)
	#define SSVPP_IMPL_VA_NUM_ARGS_1(...) SSVPP_IMPL_HAS_ZERO_OR_ONE_ARGS(__VA_ARGS__)
	#define SSVPP_IMPL_VA_NUM_ARGS_2(...) 2
	#define SSVPP_IMPL_VA_NUM_ARGS_3(...) 3
	#define SSVPP_IMPL_VA_NUM_ARGS_4(...) 4
	#define SSVPP_IMPL_VA_NUM_ARGS_5(...) 5
	#define SSVPP_IMPL_VA_NUM_ARGS_6(...) 6
	#define SSVPP_IMPL_VA_NUM_ARGS_7(...) 7
	#define SSVPP_IMPL_VA_NUM_ARGS_8(...) 8
	#define SSVPP_IMPL_VA_NUM_ARGS_9(...) 9
	#define SSVPP_IMPL_VA_NUM_ARGS_10(...) 10
	#define SSVPP_IMPL_VA_NUM_ARGS_11(...) 11
	#define SSVPP_IMPL_VA_NUM_ARGS_12(...) 12
	#define SSVPP_IMPL_VA_NUM_ARGS_13(...) 13
	#define SSVPP_IMPL_VA_NUM_ARGS_14(...) 14
	#define SSVPP_IMPL_VA_NUM_ARGS_15(...) 15
	#define SSVPP_IMPL_VA_NUM_ARGS_16(...) 16
	#define SSVPP_IMPL_VA_NUM_ARGS_17(...) 17
	#define SSVPP_IMPL_VA_NUM_ARGS_18(...) 18
	#define SSVPP_IMPL_VA_NUM_ARGS_19(...) 19
	#define SSVPP_IMPL_VA_NUM_ARGS_20(...) 20
	#define SSVPP_IMPL_VA_NUM_ARGS_21(...) 21
	#define SSVPP_IMPL_VA_NUM_ARGS_22(...) 22
	#define SSVPP_IMPL_VA_NUM_ARGS_23(...) 23
	#define SSVPP_IMPL_VA_NUM_ARGS_24(...) 24
	#define SSVPP_IMPL_VA_NUM_ARGS_25(...) 25
	#define SSVPP_IMPL_VA_NUM_ARGS_26(...) 26
	#define SSVPP_IMPL_VA_NUM_ARGS_27(...) 27
	#define SSVPP_IMPL_VA_NUM_ARGS_28(...) 28
	#define SSVPP_IMPL_VA_NUM_ARGS_29(...) 29
	#define SSVPP_IMPL_VA_NUM_ARGS_30(...) 30
	#define SSVPP_IMPL_VA_NUM_ARGS_31(...) 31
	#define SSVPP_IMPL_VA_NUM_ARGS_32(...) 32
	#define SSVPP_IMPL_VA_NUM_ARGS_33(...) 33
	#define SSVPP_IMPL_VA_NUM_ARGS_34(...) 34
	#define SSVPP_IMPL_VA_NUM_ARGS_35(...) 35
	#define SSVPP_IMPL_VA_NUM_ARGS_36(...) 36
	#define SSVPP_IMPL_VA_NUM_ARGS_37(...) 37
	#define SSVPP_IMPL_VA_NUM_ARGS_38(...) 38
	#define SSVPP_IMPL_VA_NUM_ARGS_39(...) 39
	#define SSVPP_IMPL_VA_NUM_ARGS_40(...) 40

	#define SSVPP_IMPL_VA_NUM_ARGS_REVERSE_SEQUENCE()	40, 39, 38, 37, 36, 35, 34, 33, 32, 31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21, \
														20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10,  9,  8,  7,  6,  5,  4,  3,  2,  1, \
														 0

	#define SSVPP_IMPL_VA_NUM_ARGS_DISPATCH_IMPL(	m01, m02, m03, m04, m05, m06, m07, m08, m09, m10, m11, m12, m13, m14, m15, m16, m17, m18, m19, m20, \
													m21, m22, m23, m24, m25, m26, m27, m28, m29, m30, m31, m32, m33, m34, m35, m36, m37, m38, m39, m40, \
													mN, ...) SSVPP_TKNCAT_2(SSVPP_IMPL_VA_NUM_ARGS_, mN)

	#define SSVPP_IMPL_VA_NUM_ARGS_DISPATCH(...)	SSVPP_IMPL_VA_NUM_ARGS_DISPATCH_IMPL(__VA_ARGS__)
	#define SSVPP_IMPL_VA_NUM_ARGS(...)				SSVPP_IMPL_VA_NUM_ARGS_DISPATCH(__VA_ARGS__, SSVPP_IMPL_VA_NUM_ARGS_REVERSE_SEQUENCE()) (__VA_ARGS__)

	#define SSVPP_VA_NUM_ARGS(...)					SSVPP_IMPL_VA_NUM_ARGS(__VA_ARGS__)
}

#endif
