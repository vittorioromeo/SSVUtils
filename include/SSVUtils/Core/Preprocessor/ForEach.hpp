// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_CORE_PREPROCESSOR_FOREACH
#define SSVU_CORE_PREPROCESSOR_FOREACH

#define SSVPP_IMPL_FOREACH_1(mAction, mData, mA0)				mAction(0, mData, mA0)
#define SSVPP_IMPL_FOREACH_2(mAction, mData, mA0, mA1)			mAction(1, mData, mA0) SSVPP_IMPL_FOREACH_1(mAction, mData, mA1)
#define SSVPP_IMPL_FOREACH_3(mAction, mData, mA0, mA1, ...)		mAction(2, mData, mA0) SSVPP_IMPL_FOREACH_2(mAction, mData, mA1, __VA_ARGS__)
#define SSVPP_IMPL_FOREACH_4(mAction, mData, mA0, mA1, ...)		mAction(3, mData, mA0) SSVPP_IMPL_FOREACH_3(mAction, mData, mA1, __VA_ARGS__)
#define SSVPP_IMPL_FOREACH_5(mAction, mData, mA0, mA1, ...)		mAction(4, mData, mA0) SSVPP_IMPL_FOREACH_4(mAction, mData, mA1, __VA_ARGS__)
#define SSVPP_IMPL_FOREACH_6(mAction, mData, mA0, mA1, ...)		mAction(5, mData, mA0) SSVPP_IMPL_FOREACH_5(mAction, mData, mA1, __VA_ARGS__)
#define SSVPP_IMPL_FOREACH_7(mAction, mData, mA0, mA1, ...)		mAction(6, mData, mA0) SSVPP_IMPL_FOREACH_6(mAction, mData, mA1, __VA_ARGS__)
#define SSVPP_IMPL_FOREACH_8(mAction, mData, mA0, mA1, ...)		mAction(7, mData, mA0) SSVPP_IMPL_FOREACH_7(mAction, mData, mA1, __VA_ARGS__)
#define SSVPP_IMPL_FOREACH_9(mAction, mData, mA0, mA1, ...)		mAction(8, mData, mA0) SSVPP_IMPL_FOREACH_8(mAction, mData, mA1, __VA_ARGS__)
#define SSVPP_IMPL_FOREACH_10(mAction, mData, mA0, mA1, ...)	mAction(9, mData, mA0) SSVPP_IMPL_FOREACH_9(mAction, mData, mA1, __VA_ARGS__)
#define SSVPP_IMPL_FOREACH_11(mAction, mData, mA0, mA1, ...)	mAction(10, mData, mA0) SSVPP_IMPL_FOREACH_10(mAction, mData, mA1, __VA_ARGS__)
#define SSVPP_IMPL_FOREACH_12(mAction, mData, mA0, mA1, ...)	mAction(11, mData, mA0) SSVPP_IMPL_FOREACH_11(mAction, mData, mA1, __VA_ARGS__)
#define SSVPP_IMPL_FOREACH_13(mAction, mData, mA0, mA1, ...)	mAction(12, mData, mA0) SSVPP_IMPL_FOREACH_12(mAction, mData, mA1, __VA_ARGS__)
#define SSVPP_IMPL_FOREACH_14(mAction, mData, mA0, mA1, ...)	mAction(13, mData, mA0) SSVPP_IMPL_FOREACH_13(mAction, mData, mA1, __VA_ARGS__)
#define SSVPP_IMPL_FOREACH_15(mAction, mData, mA0, mA1, ...)	mAction(14, mData, mA0) SSVPP_IMPL_FOREACH_14(mAction, mData, mA1, __VA_ARGS__)
#define SSVPP_IMPL_FOREACH_16(mAction, mData, mA0, mA1, ...)	mAction(15, mData, mA0) SSVPP_IMPL_FOREACH_15(mAction, mData, mA1, __VA_ARGS__)
#define SSVPP_IMPL_FOREACH_17(mAction, mData, mA0, mA1, ...)	mAction(16, mData, mA0) SSVPP_IMPL_FOREACH_16(mAction, mData, mA1, __VA_ARGS__)
#define SSVPP_IMPL_FOREACH_18(mAction, mData, mA0, mA1, ...)	mAction(17, mData, mA0) SSVPP_IMPL_FOREACH_17(mAction, mData, mA1, __VA_ARGS__)
#define SSVPP_IMPL_FOREACH_19(mAction, mData, mA0, mA1, ...)	mAction(18, mData, mA0) SSVPP_IMPL_FOREACH_18(mAction, mData, mA1, __VA_ARGS__)
#define SSVPP_IMPL_FOREACH_20(mAction, mData, mA0, mA1, ...)	mAction(29, mData, mA0) SSVPP_IMPL_FOREACH_19(mAction, mData, mA1, __VA_ARGS__)
#define SSVPP_IMPL_FOREACH_21(mAction, mData, mA0, mA1, ...)	mAction(20, mData, mA0) SSVPP_IMPL_FOREACH_20(mAction, mData, mA1, __VA_ARGS__)
#define SSVPP_IMPL_FOREACH_22(mAction, mData, mA0, mA1, ...)	mAction(21, mData, mA0) SSVPP_IMPL_FOREACH_21(mAction, mData, mA1, __VA_ARGS__)
#define SSVPP_IMPL_FOREACH_23(mAction, mData, mA0, mA1, ...)	mAction(22, mData, mA0) SSVPP_IMPL_FOREACH_22(mAction, mData, mA1, __VA_ARGS__)
#define SSVPP_IMPL_FOREACH_24(mAction, mData, mA0, mA1, ...)	mAction(23, mData, mA0) SSVPP_IMPL_FOREACH_23(mAction, mData, mA1, __VA_ARGS__)
#define SSVPP_IMPL_FOREACH_25(mAction, mData, mA0, mA1, ...)	mAction(24, mData, mA0) SSVPP_IMPL_FOREACH_24(mAction, mData, mA1, __VA_ARGS__)
#define SSVPP_IMPL_FOREACH_26(mAction, mData, mA0, mA1, ...)	mAction(25, mData, mA0) SSVPP_IMPL_FOREACH_25(mAction, mData, mA1, __VA_ARGS__)
#define SSVPP_IMPL_FOREACH_27(mAction, mData, mA0, mA1, ...)	mAction(26, mData, mA0) SSVPP_IMPL_FOREACH_26(mAction, mData, mA1, __VA_ARGS__)
#define SSVPP_IMPL_FOREACH_28(mAction, mData, mA0, mA1, ...)	mAction(27, mData, mA0) SSVPP_IMPL_FOREACH_27(mAction, mData, mA1, __VA_ARGS__)
#define SSVPP_IMPL_FOREACH_29(mAction, mData, mA0, mA1, ...)	mAction(28, mData, mA0) SSVPP_IMPL_FOREACH_28(mAction, mData, mA1, __VA_ARGS__)
#define SSVPP_IMPL_FOREACH_30(mAction, mData, mA0, mA1, ...)	mAction(29, mData, mA0) SSVPP_IMPL_FOREACH_29(mAction, mData, mA1, __VA_ARGS__)
#define SSVPP_IMPL_FOREACH_31(mAction, mData, mA0, mA1, ...)	mAction(30, mData, mA0) SSVPP_IMPL_FOREACH_30(mAction, mData, mA1, __VA_ARGS__)
#define SSVPP_IMPL_FOREACH_32(mAction, mData, mA0, mA1, ...)	mAction(31, mData, mA0) SSVPP_IMPL_FOREACH_31(mAction, mData, mA1, __VA_ARGS__)
#define SSVPP_IMPL_FOREACH_33(mAction, mData, mA0, mA1, ...)	mAction(32, mData, mA0) SSVPP_IMPL_FOREACH_32(mAction, mData, mA1, __VA_ARGS__)
#define SSVPP_IMPL_FOREACH_34(mAction, mData, mA0, mA1, ...)	mAction(33, mData, mA0) SSVPP_IMPL_FOREACH_33(mAction, mData, mA1, __VA_ARGS__)
#define SSVPP_IMPL_FOREACH_35(mAction, mData, mA0, mA1, ...)	mAction(34, mData, mA0) SSVPP_IMPL_FOREACH_34(mAction, mData, mA1, __VA_ARGS__)
#define SSVPP_IMPL_FOREACH_36(mAction, mData, mA0, mA1, ...)	mAction(35, mData, mA0) SSVPP_IMPL_FOREACH_35(mAction, mData, mA1, __VA_ARGS__)
#define SSVPP_IMPL_FOREACH_37(mAction, mData, mA0, mA1, ...)	mAction(36, mData, mA0) SSVPP_IMPL_FOREACH_36(mAction, mData, mA1, __VA_ARGS__)
#define SSVPP_IMPL_FOREACH_38(mAction, mData, mA0, mA1, ...)	mAction(37, mData, mA0) SSVPP_IMPL_FOREACH_37(mAction, mData, mA1, __VA_ARGS__)
#define SSVPP_IMPL_FOREACH_39(mAction, mData, mA0, mA1, ...)	mAction(38, mData, mA0) SSVPP_IMPL_FOREACH_38(mAction, mData, mA1, __VA_ARGS__)
#define SSVPP_IMPL_FOREACH_40(mAction, mData, mA0, mA1, ...)	mAction(39, mData, mA0) SSVPP_IMPL_FOREACH_39(mAction, mData, mA1, __VA_ARGS__)

#define SSVPP_IMPL_FOREACH(mAction, mData, ...)	SSVPP_TKNCAT_2(SSVPP_IMPL_FOREACH_, SSVPP_ARGCOUNT(__VA_ARGS__)) (mAction, mData, __VA_ARGS__)
#define SSVPP_FOREACH(...)						SSVPP_IMPL_FOREACH(__VA_ARGS__)

#endif
