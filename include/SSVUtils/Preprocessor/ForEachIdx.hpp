// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_PREPROCESSOR_FOREACH_IDX
#define SSVU_PREPROCESSOR_FOREACH_IDX

#include "SSVUtils/Preprocessor/Utils.hpp"

namespace ssvu
{
	#define SSVU_PP_IMPL_FOREACH_IDX_0(...)					SSVU_PP_EMPTY()
	#define SSVU_PP_IMPL_FOREACH_IDX_1(mAction, mArg)		mAction(0, mArg)
	#define SSVU_PP_IMPL_FOREACH_IDX_2(mAction, mArg, ...)	mAction(1, mArg) SSVU_PP_IMPL_FOREACH_IDX_1(mAction, __VA_ARGS__)
	#define SSVU_PP_IMPL_FOREACH_IDX_3(mAction, mArg, ...)	mAction(2, mArg) SSVU_PP_IMPL_FOREACH_IDX_2(mAction, __VA_ARGS__)
	#define SSVU_PP_IMPL_FOREACH_IDX_4(mAction, mArg, ...)	mAction(3, mArg) SSVU_PP_IMPL_FOREACH_IDX_3(mAction, __VA_ARGS__)
	#define SSVU_PP_IMPL_FOREACH_IDX_5(mAction, mArg, ...)	mAction(4, mArg) SSVU_PP_IMPL_FOREACH_IDX_4(mAction, __VA_ARGS__)
	#define SSVU_PP_IMPL_FOREACH_IDX_6(mAction, mArg, ...)	mAction(5, mArg) SSVU_PP_IMPL_FOREACH_IDX_5(mAction, __VA_ARGS__)
	#define SSVU_PP_IMPL_FOREACH_IDX_7(mAction, mArg, ...)	mAction(6, mArg) SSVU_PP_IMPL_FOREACH_IDX_6(mAction, __VA_ARGS__)
	#define SSVU_PP_IMPL_FOREACH_IDX_8(mAction, mArg, ...)	mAction(7, mArg) SSVU_PP_IMPL_FOREACH_IDX_7(mAction, __VA_ARGS__)
	#define SSVU_PP_IMPL_FOREACH_IDX_9(mAction, mArg, ...)	mAction(8, mArg) SSVU_PP_IMPL_FOREACH_IDX_8(mAction, __VA_ARGS__)
	#define SSVU_PP_IMPL_FOREACH_IDX_10(mAction, mArg, ...)	mAction(9, mArg) SSVU_PP_IMPL_FOREACH_IDX_9(mAction, __VA_ARGS__)
	#define SSVU_PP_IMPL_FOREACH_IDX_11(mAction, mArg, ...)	mAction(10, mArg) SSVU_PP_IMPL_FOREACH_IDX_10(mAction, __VA_ARGS__)
	#define SSVU_PP_IMPL_FOREACH_IDX_12(mAction, mArg, ...)	mAction(11, mArg) SSVU_PP_IMPL_FOREACH_IDX_11(mAction, __VA_ARGS__)
	#define SSVU_PP_IMPL_FOREACH_IDX_13(mAction, mArg, ...)	mAction(12, mArg) SSVU_PP_IMPL_FOREACH_IDX_12(mAction, __VA_ARGS__)
	#define SSVU_PP_IMPL_FOREACH_IDX_14(mAction, mArg, ...)	mAction(13, mArg) SSVU_PP_IMPL_FOREACH_IDX_13(mAction, __VA_ARGS__)
	#define SSVU_PP_IMPL_FOREACH_IDX_15(mAction, mArg, ...)	mAction(14, mArg) SSVU_PP_IMPL_FOREACH_IDX_14(mAction, __VA_ARGS__)
	#define SSVU_PP_IMPL_FOREACH_IDX_16(mAction, mArg, ...)	mAction(15, mArg) SSVU_PP_IMPL_FOREACH_IDX_15(mAction, __VA_ARGS__)
	#define SSVU_PP_IMPL_FOREACH_IDX_17(mAction, mArg, ...)	mAction(16, mArg) SSVU_PP_IMPL_FOREACH_IDX_16(mAction, __VA_ARGS__)
	#define SSVU_PP_IMPL_FOREACH_IDX_18(mAction, mArg, ...)	mAction(17, mArg) SSVU_PP_IMPL_FOREACH_IDX_17(mAction, __VA_ARGS__)
	#define SSVU_PP_IMPL_FOREACH_IDX_19(mAction, mArg, ...)	mAction(18, mArg) SSVU_PP_IMPL_FOREACH_IDX_18(mAction, __VA_ARGS__)
	#define SSVU_PP_IMPL_FOREACH_IDX_20(mAction, mArg, ...)	mAction(29, mArg) SSVU_PP_IMPL_FOREACH_IDX_19(mAction, __VA_ARGS__)
	#define SSVU_PP_IMPL_FOREACH_IDX_21(mAction, mArg, ...)	mAction(20, mArg) SSVU_PP_IMPL_FOREACH_IDX_20(mAction, __VA_ARGS__)
	#define SSVU_PP_IMPL_FOREACH_IDX_22(mAction, mArg, ...)	mAction(21, mArg) SSVU_PP_IMPL_FOREACH_IDX_21(mAction, __VA_ARGS__)
	#define SSVU_PP_IMPL_FOREACH_IDX_23(mAction, mArg, ...)	mAction(22, mArg) SSVU_PP_IMPL_FOREACH_IDX_22(mAction, __VA_ARGS__)
	#define SSVU_PP_IMPL_FOREACH_IDX_24(mAction, mArg, ...)	mAction(23, mArg) SSVU_PP_IMPL_FOREACH_IDX_23(mAction, __VA_ARGS__)
	#define SSVU_PP_IMPL_FOREACH_IDX_25(mAction, mArg, ...)	mAction(24, mArg) SSVU_PP_IMPL_FOREACH_IDX_24(mAction, __VA_ARGS__)
	#define SSVU_PP_IMPL_FOREACH_IDX_26(mAction, mArg, ...)	mAction(25, mArg) SSVU_PP_IMPL_FOREACH_IDX_25(mAction, __VA_ARGS__)
	#define SSVU_PP_IMPL_FOREACH_IDX_27(mAction, mArg, ...)	mAction(26, mArg) SSVU_PP_IMPL_FOREACH_IDX_26(mAction, __VA_ARGS__)
	#define SSVU_PP_IMPL_FOREACH_IDX_28(mAction, mArg, ...)	mAction(27, mArg) SSVU_PP_IMPL_FOREACH_IDX_27(mAction, __VA_ARGS__)
	#define SSVU_PP_IMPL_FOREACH_IDX_29(mAction, mArg, ...)	mAction(28, mArg) SSVU_PP_IMPL_FOREACH_IDX_28(mAction, __VA_ARGS__)
	#define SSVU_PP_IMPL_FOREACH_IDX_30(mAction, mArg, ...)	mAction(29, mArg) SSVU_PP_IMPL_FOREACH_IDX_29(mAction, __VA_ARGS__)
	#define SSVU_PP_IMPL_FOREACH_IDX_31(mAction, mArg, ...)	mAction(30, mArg) SSVU_PP_IMPL_FOREACH_IDX_30(mAction, __VA_ARGS__)
	#define SSVU_PP_IMPL_FOREACH_IDX_32(mAction, mArg, ...)	mAction(31, mArg) SSVU_PP_IMPL_FOREACH_IDX_31(mAction, __VA_ARGS__)
	#define SSVU_PP_IMPL_FOREACH_IDX_33(mAction, mArg, ...)	mAction(32, mArg) SSVU_PP_IMPL_FOREACH_IDX_32(mAction, __VA_ARGS__)
	#define SSVU_PP_IMPL_FOREACH_IDX_34(mAction, mArg, ...)	mAction(33, mArg) SSVU_PP_IMPL_FOREACH_IDX_33(mAction, __VA_ARGS__)
	#define SSVU_PP_IMPL_FOREACH_IDX_35(mAction, mArg, ...)	mAction(34, mArg) SSVU_PP_IMPL_FOREACH_IDX_34(mAction, __VA_ARGS__)
	#define SSVU_PP_IMPL_FOREACH_IDX_36(mAction, mArg, ...)	mAction(35, mArg) SSVU_PP_IMPL_FOREACH_IDX_35(mAction, __VA_ARGS__)
	#define SSVU_PP_IMPL_FOREACH_IDX_37(mAction, mArg, ...)	mAction(36, mArg) SSVU_PP_IMPL_FOREACH_IDX_36(mAction, __VA_ARGS__)
	#define SSVU_PP_IMPL_FOREACH_IDX_38(mAction, mArg, ...)	mAction(37, mArg) SSVU_PP_IMPL_FOREACH_IDX_37(mAction, __VA_ARGS__)
	#define SSVU_PP_IMPL_FOREACH_IDX_39(mAction, mArg, ...)	mAction(38, mArg) SSVU_PP_IMPL_FOREACH_IDX_38(mAction, __VA_ARGS__)
	#define SSVU_PP_IMPL_FOREACH_IDX_40(mAction, mArg, ...)	mAction(39, mArg) SSVU_PP_IMPL_FOREACH_IDX_39(mAction, __VA_ARGS__)

	#define SSVU_PP_IMPL_FOREACH_IDX_REVERSE_SEQUENCE()	40, 39, 38, 37, 36, 35, 34, 33, 32, 31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21, \
														20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10,  9,  8,  7,  6,  5,  4,  3,  2,  1, \
														 0

	#define SSVU_PP_IMPL_FOREACH_IDX_DISPATCH_IMPL(	m01, m02, m03, m04, m05, m06, m07, m08, m09, m10, m11, m12, m13, m14, m15, m16, m17, m18, m19, m20, \
													m21, m22, m23, m24, m25, m26, m27, m28, m29, m30, m31, m32, m33, m34, m35, m36, m37, m38, m39, m40, \
													mN, ...) SSVU_PP_TOKENPASTE_2(SSVU_PP_IMPL_FOREACH_IDX_, mN)


	#define SSVU_PP_IMPL_FOREACH_IDX_DISPATCH(...)	SSVU_PP_IMPL_FOREACH_IDX_DISPATCH_IMPL(__VA_ARGS__)
	#define SSVU_PP_IMPL_FOREACH_IDX(mAction, ...)	SSVU_PP_IMPL_FOREACH_IDX_DISPATCH(__VA_ARGS__, SSVU_PP_IMPL_FOREACH_IDX_REVERSE_SEQUENCE()) (mAction, __VA_ARGS__)

	#define SSVU_PP_FOREACH_IDX(...)				SSVU_PP_IMPL_FOREACH_IDX(__VA_ARGS__)
}

#endif
