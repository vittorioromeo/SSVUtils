// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_PREPROCESSOR_FOREACH
#define SSVU_PREPROCESSOR_FOREACH

#include "SSVUtils/Preprocessor/Utils.hpp"

namespace ssvu
{
	#define SSVU_PP_IMPL_FOREACH_0(...)							SSVU_PP_EMPTY()
	#define SSVU_PP_IMPL_FOREACH_1(mAction, mData, mArg)		mAction(0, mData, mArg)
	#define SSVU_PP_IMPL_FOREACH_2(mAction, mData, mArg, ...)	mAction(1, mData, mArg) SSVU_PP_IMPL_FOREACH_1(mAction, mData, __VA_ARGS__)
	#define SSVU_PP_IMPL_FOREACH_3(mAction, mData, mArg, ...)	mAction(2, mData, mArg) SSVU_PP_IMPL_FOREACH_2(mAction, mData, __VA_ARGS__)
	#define SSVU_PP_IMPL_FOREACH_4(mAction, mData, mArg, ...)	mAction(3, mData, mArg) SSVU_PP_IMPL_FOREACH_3(mAction, mData, __VA_ARGS__)
	#define SSVU_PP_IMPL_FOREACH_5(mAction, mData, mArg, ...)	mAction(4, mData, mArg) SSVU_PP_IMPL_FOREACH_4(mAction, mData, __VA_ARGS__)
	#define SSVU_PP_IMPL_FOREACH_6(mAction, mData, mArg, ...)	mAction(5, mData, mArg) SSVU_PP_IMPL_FOREACH_5(mAction, mData, __VA_ARGS__)
	#define SSVU_PP_IMPL_FOREACH_7(mAction, mData, mArg, ...)	mAction(6, mData, mArg) SSVU_PP_IMPL_FOREACH_6(mAction, mData, __VA_ARGS__)
	#define SSVU_PP_IMPL_FOREACH_8(mAction, mData, mArg, ...)	mAction(7, mData, mArg) SSVU_PP_IMPL_FOREACH_7(mAction, mData, __VA_ARGS__)
	#define SSVU_PP_IMPL_FOREACH_9(mAction, mData, mArg, ...)	mAction(8, mData, mArg) SSVU_PP_IMPL_FOREACH_8(mAction, mData, __VA_ARGS__)
	#define SSVU_PP_IMPL_FOREACH_10(mAction, mData, mArg, ...)	mAction(9, mData, mArg) SSVU_PP_IMPL_FOREACH_9(mAction, mData, __VA_ARGS__)
	#define SSVU_PP_IMPL_FOREACH_11(mAction, mData, mArg, ...)	mAction(10, mData, mArg) SSVU_PP_IMPL_FOREACH_10(mAction, mData, __VA_ARGS__)
	#define SSVU_PP_IMPL_FOREACH_12(mAction, mData, mArg, ...)	mAction(11, mData, mArg) SSVU_PP_IMPL_FOREACH_11(mAction, mData, __VA_ARGS__)
	#define SSVU_PP_IMPL_FOREACH_13(mAction, mData, mArg, ...)	mAction(12, mData, mArg) SSVU_PP_IMPL_FOREACH_12(mAction, mData, __VA_ARGS__)
	#define SSVU_PP_IMPL_FOREACH_14(mAction, mData, mArg, ...)	mAction(13, mData, mArg) SSVU_PP_IMPL_FOREACH_13(mAction, mData, __VA_ARGS__)
	#define SSVU_PP_IMPL_FOREACH_15(mAction, mData, mArg, ...)	mAction(14, mData, mArg) SSVU_PP_IMPL_FOREACH_14(mAction, mData, __VA_ARGS__)
	#define SSVU_PP_IMPL_FOREACH_16(mAction, mData, mArg, ...)	mAction(15, mData, mArg) SSVU_PP_IMPL_FOREACH_15(mAction, mData, __VA_ARGS__)
	#define SSVU_PP_IMPL_FOREACH_17(mAction, mData, mArg, ...)	mAction(16, mData, mArg) SSVU_PP_IMPL_FOREACH_16(mAction, mData, __VA_ARGS__)
	#define SSVU_PP_IMPL_FOREACH_18(mAction, mData, mArg, ...)	mAction(17, mData, mArg) SSVU_PP_IMPL_FOREACH_17(mAction, mData, __VA_ARGS__)
	#define SSVU_PP_IMPL_FOREACH_19(mAction, mData, mArg, ...)	mAction(18, mData, mArg) SSVU_PP_IMPL_FOREACH_18(mAction, mData, __VA_ARGS__)
	#define SSVU_PP_IMPL_FOREACH_20(mAction, mData, mArg, ...)	mAction(29, mData, mArg) SSVU_PP_IMPL_FOREACH_19(mAction, mData, __VA_ARGS__)
	#define SSVU_PP_IMPL_FOREACH_21(mAction, mData, mArg, ...)	mAction(20, mData, mArg) SSVU_PP_IMPL_FOREACH_20(mAction, mData, __VA_ARGS__)
	#define SSVU_PP_IMPL_FOREACH_22(mAction, mData, mArg, ...)	mAction(21, mData, mArg) SSVU_PP_IMPL_FOREACH_21(mAction, mData, __VA_ARGS__)
	#define SSVU_PP_IMPL_FOREACH_23(mAction, mData, mArg, ...)	mAction(22, mData, mArg) SSVU_PP_IMPL_FOREACH_22(mAction, mData, __VA_ARGS__)
	#define SSVU_PP_IMPL_FOREACH_24(mAction, mData, mArg, ...)	mAction(23, mData, mArg) SSVU_PP_IMPL_FOREACH_23(mAction, mData, __VA_ARGS__)
	#define SSVU_PP_IMPL_FOREACH_25(mAction, mData, mArg, ...)	mAction(24, mData, mArg) SSVU_PP_IMPL_FOREACH_24(mAction, mData, __VA_ARGS__)
	#define SSVU_PP_IMPL_FOREACH_26(mAction, mData, mArg, ...)	mAction(25, mData, mArg) SSVU_PP_IMPL_FOREACH_25(mAction, mData, __VA_ARGS__)
	#define SSVU_PP_IMPL_FOREACH_27(mAction, mData, mArg, ...)	mAction(26, mData, mArg) SSVU_PP_IMPL_FOREACH_26(mAction, mData, __VA_ARGS__)
	#define SSVU_PP_IMPL_FOREACH_28(mAction, mData, mArg, ...)	mAction(27, mData, mArg) SSVU_PP_IMPL_FOREACH_27(mAction, mData, __VA_ARGS__)
	#define SSVU_PP_IMPL_FOREACH_29(mAction, mData, mArg, ...)	mAction(28, mData, mArg) SSVU_PP_IMPL_FOREACH_28(mAction, mData, __VA_ARGS__)
	#define SSVU_PP_IMPL_FOREACH_30(mAction, mData, mArg, ...)	mAction(29, mData, mArg) SSVU_PP_IMPL_FOREACH_29(mAction, mData, __VA_ARGS__)
	#define SSVU_PP_IMPL_FOREACH_31(mAction, mData, mArg, ...)	mAction(30, mData, mArg) SSVU_PP_IMPL_FOREACH_30(mAction, mData, __VA_ARGS__)
	#define SSVU_PP_IMPL_FOREACH_32(mAction, mData, mArg, ...)	mAction(31, mData, mArg) SSVU_PP_IMPL_FOREACH_31(mAction, mData, __VA_ARGS__)
	#define SSVU_PP_IMPL_FOREACH_33(mAction, mData, mArg, ...)	mAction(32, mData, mArg) SSVU_PP_IMPL_FOREACH_32(mAction, mData, __VA_ARGS__)
	#define SSVU_PP_IMPL_FOREACH_34(mAction, mData, mArg, ...)	mAction(33, mData, mArg) SSVU_PP_IMPL_FOREACH_33(mAction, mData, __VA_ARGS__)
	#define SSVU_PP_IMPL_FOREACH_35(mAction, mData, mArg, ...)	mAction(34, mData, mArg) SSVU_PP_IMPL_FOREACH_34(mAction, mData, __VA_ARGS__)
	#define SSVU_PP_IMPL_FOREACH_36(mAction, mData, mArg, ...)	mAction(35, mData, mArg) SSVU_PP_IMPL_FOREACH_35(mAction, mData, __VA_ARGS__)
	#define SSVU_PP_IMPL_FOREACH_37(mAction, mData, mArg, ...)	mAction(36, mData, mArg) SSVU_PP_IMPL_FOREACH_36(mAction, mData, __VA_ARGS__)
	#define SSVU_PP_IMPL_FOREACH_38(mAction, mData, mArg, ...)	mAction(37, mData, mArg) SSVU_PP_IMPL_FOREACH_37(mAction, mData, __VA_ARGS__)
	#define SSVU_PP_IMPL_FOREACH_39(mAction, mData, mArg, ...)	mAction(38, mData, mArg) SSVU_PP_IMPL_FOREACH_38(mAction, mData, __VA_ARGS__)
	#define SSVU_PP_IMPL_FOREACH_40(mAction, mData, mArg, ...)	mAction(39, mData, mArg) SSVU_PP_IMPL_FOREACH_39(mAction, mData, __VA_ARGS__)

	#define SSVU_PP_IMPL_FOREACH_REVERSE_SEQUENCE()	40, 39, 38, 37, 36, 35, 34, 33, 32, 31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21, \
													20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10,  9,  8,  7,  6,  5,  4,  3,  2,  1, \
													 0

	#define SSVU_PP_IMPL_FOREACH_DISPATCH_IMPL(	m01, m02, m03, m04, m05, m06, m07, m08, m09, m10, m11, m12, m13, m14, m15, m16, m17, m18, m19, m20, \
												m21, m22, m23, m24, m25, m26, m27, m28, m29, m30, m31, m32, m33, m34, m35, m36, m37, m38, m39, m40, \
												mN, ...) SSVU_PP_TOKENPASTE_2(SSVU_PP_IMPL_FOREACH_, mN)


	#define SSVU_PP_IMPL_FOREACH_DISPATCH(...)			SSVU_PP_IMPL_FOREACH_DISPATCH_IMPL(__VA_ARGS__)
	#define SSVU_PP_IMPL_FOREACH(mAction, mData, ...)	SSVU_PP_IMPL_FOREACH_DISPATCH(__VA_ARGS__, SSVU_PP_IMPL_FOREACH_REVERSE_SEQUENCE()) (mAction, mData, __VA_ARGS__)

	#define SSVU_PP_FOREACH(...)						SSVU_PP_IMPL_FOREACH(__VA_ARGS__)
}

#endif
