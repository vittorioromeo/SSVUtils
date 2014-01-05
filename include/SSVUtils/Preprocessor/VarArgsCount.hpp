// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_PREPROCESSOR_VARARGSCOUNT
#define SSVU_PREPROCESSOR_VARARGSCOUNT

#include <SSVUtils/Preprocessor/Utils.hpp>

namespace ssvu
{
	#define SSVU_PP_IMPL_ARG_40(m00, m01, m02, m03, m04, m05, m06, m07, m08, m09, m10, m11, m12, m13, m14, m15, m16, m17, m18, m19, m20, \
								m21, m22, m23, m24, m25, m26, m27, m28, m29, m30, m31, m32, m33, m34, m35, m36, m37, m38, m39, ...) m39

	#define SSVU_PP_IMPL_HAS_COMMA_40(...)		SSVU_PP_IMPL_ARG_40(__VA_ARGS__, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0)
	#define SSVU_PP_IMPL_HAS_NO_COMMA_40(...)	SSVU_PP_IMPL_ARG_40(__VA_ARGS__, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1)

	#define SSVU_PP_IMPL_TRIGGER_PARENTHESIS(...) ,

	#define SSVU_PP_IMPL_HAS_ZERO_OR_ONE_ARGS_IMPL(m0, m1, m2, m3) SSVU_PP_IMPL_HAS_NO_COMMA_40(SSVU_PP_TOKENPASTE_5(SSVU_PP_IMPL_IS_EMPTY_CASE_, m0, m1, m2, m3))
	#define SSVU_PP_IMPL_HAS_ZERO_OR_ONE_ARGS(...)	SSVU_PP_IMPL_HAS_ZERO_OR_ONE_ARGS_IMPL(SSVU_PP_IMPL_HAS_COMMA_40(__VA_ARGS__), \
													SSVU_PP_IMPL_HAS_COMMA_40(SSVU_PP_IMPL_TRIGGER_PARENTHESIS __VA_ARGS__), \
													SSVU_PP_IMPL_HAS_COMMA_40(__VA_ARGS__ (~)), \
													SSVU_PP_IMPL_HAS_COMMA_40(SSVU_PP_IMPL_TRIGGER_PARENTHESIS __VA_ARGS__ (~)))

	#define SSVU_PP_IMPL_IS_EMPTY_CASE_0001 ,

	#define SSVU_PP_IMPL_VA_NUM_ARGS_0(...) SSVU_PP_IMPL_HAS_ZERO_OR_ONE_ARGS(__VA_ARGS__)
	#define SSVU_PP_IMPL_VA_NUM_ARGS_1(...) SSVU_PP_IMPL_HAS_ZERO_OR_ONE_ARGS(__VA_ARGS__)
	#define SSVU_PP_IMPL_VA_NUM_ARGS_2(...) 2
	#define SSVU_PP_IMPL_VA_NUM_ARGS_3(...) 3
	#define SSVU_PP_IMPL_VA_NUM_ARGS_4(...) 4
	#define SSVU_PP_IMPL_VA_NUM_ARGS_5(...) 5
	#define SSVU_PP_IMPL_VA_NUM_ARGS_6(...) 6
	#define SSVU_PP_IMPL_VA_NUM_ARGS_7(...) 7
	#define SSVU_PP_IMPL_VA_NUM_ARGS_8(...) 8
	#define SSVU_PP_IMPL_VA_NUM_ARGS_9(...) 9
	#define SSVU_PP_IMPL_VA_NUM_ARGS_10(...) 10
	#define SSVU_PP_IMPL_VA_NUM_ARGS_11(...) 11
	#define SSVU_PP_IMPL_VA_NUM_ARGS_12(...) 12
	#define SSVU_PP_IMPL_VA_NUM_ARGS_13(...) 13
	#define SSVU_PP_IMPL_VA_NUM_ARGS_14(...) 14
	#define SSVU_PP_IMPL_VA_NUM_ARGS_15(...) 15
	#define SSVU_PP_IMPL_VA_NUM_ARGS_16(...) 16
	#define SSVU_PP_IMPL_VA_NUM_ARGS_17(...) 17
	#define SSVU_PP_IMPL_VA_NUM_ARGS_18(...) 18
	#define SSVU_PP_IMPL_VA_NUM_ARGS_19(...) 19
	#define SSVU_PP_IMPL_VA_NUM_ARGS_20(...) 20
	#define SSVU_PP_IMPL_VA_NUM_ARGS_21(...) 21
	#define SSVU_PP_IMPL_VA_NUM_ARGS_22(...) 22
	#define SSVU_PP_IMPL_VA_NUM_ARGS_23(...) 23
	#define SSVU_PP_IMPL_VA_NUM_ARGS_24(...) 24
	#define SSVU_PP_IMPL_VA_NUM_ARGS_25(...) 25
	#define SSVU_PP_IMPL_VA_NUM_ARGS_26(...) 26
	#define SSVU_PP_IMPL_VA_NUM_ARGS_27(...) 27
	#define SSVU_PP_IMPL_VA_NUM_ARGS_28(...) 28
	#define SSVU_PP_IMPL_VA_NUM_ARGS_29(...) 29
	#define SSVU_PP_IMPL_VA_NUM_ARGS_30(...) 30
	#define SSVU_PP_IMPL_VA_NUM_ARGS_31(...) 31
	#define SSVU_PP_IMPL_VA_NUM_ARGS_32(...) 32
	#define SSVU_PP_IMPL_VA_NUM_ARGS_33(...) 33
	#define SSVU_PP_IMPL_VA_NUM_ARGS_34(...) 34
	#define SSVU_PP_IMPL_VA_NUM_ARGS_35(...) 35
	#define SSVU_PP_IMPL_VA_NUM_ARGS_36(...) 36
	#define SSVU_PP_IMPL_VA_NUM_ARGS_37(...) 37
	#define SSVU_PP_IMPL_VA_NUM_ARGS_38(...) 38
	#define SSVU_PP_IMPL_VA_NUM_ARGS_39(...) 39
	#define SSVU_PP_IMPL_VA_NUM_ARGS_40(...) 40

	#define SSVU_PP_VA_NUM_ARGS_REVERSE_SEQUENCE(...)	SSVU_PP_IMPL_VA_NUM_ARGS_40(__VA_ARGS__), \
														SSVU_PP_IMPL_VA_NUM_ARGS_39(__VA_ARGS__), \
														SSVU_PP_IMPL_VA_NUM_ARGS_38(__VA_ARGS__), \
														SSVU_PP_IMPL_VA_NUM_ARGS_37(__VA_ARGS__), \
														SSVU_PP_IMPL_VA_NUM_ARGS_36(__VA_ARGS__), \
														SSVU_PP_IMPL_VA_NUM_ARGS_35(__VA_ARGS__), \
														SSVU_PP_IMPL_VA_NUM_ARGS_34(__VA_ARGS__), \
														SSVU_PP_IMPL_VA_NUM_ARGS_33(__VA_ARGS__), \
														SSVU_PP_IMPL_VA_NUM_ARGS_32(__VA_ARGS__), \
														SSVU_PP_IMPL_VA_NUM_ARGS_31(__VA_ARGS__), \
														SSVU_PP_IMPL_VA_NUM_ARGS_30(__VA_ARGS__), \
														SSVU_PP_IMPL_VA_NUM_ARGS_29(__VA_ARGS__), \
														SSVU_PP_IMPL_VA_NUM_ARGS_28(__VA_ARGS__), \
														SSVU_PP_IMPL_VA_NUM_ARGS_27(__VA_ARGS__), \
														SSVU_PP_IMPL_VA_NUM_ARGS_26(__VA_ARGS__), \
														SSVU_PP_IMPL_VA_NUM_ARGS_25(__VA_ARGS__), \
														SSVU_PP_IMPL_VA_NUM_ARGS_24(__VA_ARGS__), \
														SSVU_PP_IMPL_VA_NUM_ARGS_23(__VA_ARGS__), \
														SSVU_PP_IMPL_VA_NUM_ARGS_22(__VA_ARGS__), \
														SSVU_PP_IMPL_VA_NUM_ARGS_21(__VA_ARGS__), \
														SSVU_PP_IMPL_VA_NUM_ARGS_20(__VA_ARGS__), \
														SSVU_PP_IMPL_VA_NUM_ARGS_19(__VA_ARGS__), \
														SSVU_PP_IMPL_VA_NUM_ARGS_18(__VA_ARGS__), \
														SSVU_PP_IMPL_VA_NUM_ARGS_17(__VA_ARGS__), \
														SSVU_PP_IMPL_VA_NUM_ARGS_16(__VA_ARGS__), \
														SSVU_PP_IMPL_VA_NUM_ARGS_15(__VA_ARGS__), \
														SSVU_PP_IMPL_VA_NUM_ARGS_14(__VA_ARGS__), \
														SSVU_PP_IMPL_VA_NUM_ARGS_13(__VA_ARGS__), \
														SSVU_PP_IMPL_VA_NUM_ARGS_12(__VA_ARGS__), \
														SSVU_PP_IMPL_VA_NUM_ARGS_11(__VA_ARGS__), \
														SSVU_PP_IMPL_VA_NUM_ARGS_10(__VA_ARGS__), \
														SSVU_PP_IMPL_VA_NUM_ARGS_9(__VA_ARGS__), \
														SSVU_PP_IMPL_VA_NUM_ARGS_8(__VA_ARGS__), \
														SSVU_PP_IMPL_VA_NUM_ARGS_7(__VA_ARGS__), \
														SSVU_PP_IMPL_VA_NUM_ARGS_6(__VA_ARGS__), \
														SSVU_PP_IMPL_VA_NUM_ARGS_5(__VA_ARGS__), \
														SSVU_PP_IMPL_VA_NUM_ARGS_4(__VA_ARGS__), \
														SSVU_PP_IMPL_VA_NUM_ARGS_3(__VA_ARGS__), \
														SSVU_PP_IMPL_VA_NUM_ARGS_2(__VA_ARGS__), \
														SSVU_PP_IMPL_VA_NUM_ARGS_1(__VA_ARGS__), \
														SSVU_PP_IMPL_VA_NUM_ARGS_0(__VA_ARGS__)

	#define SSVU_PP_IMPL_VA_NUM_ARGS_DISPATCH_IMPL(	m01, m02, m03, m04, m05, m06, m07, m08, m09, m10, m11, m12, m13, m14, m15, m16, m17, m18, m19, m20, \
													m21, m22, m23, m24, m25, m26, m27, m28, m29, m30, m31, m32, m33, m34, m35, m36, m37, m38, m39, m40, mN, ...) mN

	#define SSVU_PP_IMPL_VA_NUM_ARGS_DISPATCH(...)	SSVU_PP_IMPL_VA_NUM_ARGS_DISPATCH_IMPL(__VA_ARGS__)
	#define SSVU_PP_IMPL_VA_NUM_ARGS_IMPL(...)		SSVU_PP_IMPL_VA_NUM_ARGS_DISPATCH(__VA_ARGS__, SSVU_PP_VA_NUM_ARGS_REVERSE_SEQUENCE(__VA_ARGS__))

	#define SSVU_PP_VA_NUM_ARGS(...)				SSVU_PP_IMPL_VA_NUM_ARGS_IMPL(__VA_ARGS__)
}

#endif
