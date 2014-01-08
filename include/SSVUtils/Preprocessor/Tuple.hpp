// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_PREPROCESSOR_TUPLE
#define SSVU_PREPROCESSOR_TUPLE

#include "SSVUtils/Preprocessor/VarArgsCount.hpp"
#include "SSVUtils/Preprocessor/Utils.hpp"

namespace ssvu
{
	#define SSVPP_IMPL_TPL_GET_0(m0, ...) m0
	#define SSVPP_IMPL_TPL_GET_1(m0, m1, ...) m1
	#define SSVPP_IMPL_TPL_GET_2(m0, m1, m2, ...) m2
	#define SSVPP_IMPL_TPL_GET_3(m0, m1, m2, m3, ...) m3
	#define SSVPP_IMPL_TPL_GET_4(m0, m1, m2, m3, m4, ...) m4
	#define SSVPP_IMPL_TPL_GET_5(m0, m1, m2, m3, m4, m5, ...) m5
	#define SSVPP_IMPL_TPL_GET_6(m0, m1, m2, m3, m4, m5, m6, ...) m6
	#define SSVPP_IMPL_TPL_GET_7(m0, m1, m2, m3, m4, m5, m6, m7, ...) m7
	#define SSVPP_IMPL_TPL_GET_8(m0, m1, m2, m3, m4, m5, m6, m7, m8, ...) m8
	#define SSVPP_IMPL_TPL_GET_9(m0, m1, m2, m3, m4, m5, m6, m7, m8, m9, ...) m9
	#define SSVPP_IMPL_TPL_GET_10(m0, m1, m2, m3, m4, m5, m6, m7, m8, m9, m10, ...) m10
	#define SSVPP_IMPL_TPL_GET_11(m0, m1, m2, m3, m4, m5, m6, m7, m8, m9, m10, m11, ...) m11
	#define SSVPP_IMPL_TPL_GET_12(m0, m1, m2, m3, m4, m5, m6, m7, m8, m9, m10, m11, m12, ...) m12
	#define SSVPP_IMPL_TPL_GET_13(m0, m1, m2, m3, m4, m5, m6, m7, m8, m9, m10, m11, m12, m13, ...) m13
	#define SSVPP_IMPL_TPL_GET_14(m0, m1, m2, m3, m4, m5, m6, m7, m8, m9, m10, m11, m12, m13, m14, ...) m14
	#define SSVPP_IMPL_TPL_GET_15(m0, m1, m2, m3, m4, m5, m6, m7, m8, m9, m10, m11, m12, m13, m14, m15, ...) m15
	#define SSVPP_IMPL_TPL_GET_16(m0, m1, m2, m3, m4, m5, m6, m7, m8, m9, m10, m11, m12, m13, m14, m15, m16, ...) m16
	#define SSVPP_IMPL_TPL_GET_17(m0, m1, m2, m3, m4, m5, m6, m7, m8, m9, m10, m11, m12, m13, m14, m15, m16, m17, ...) m17
	#define SSVPP_IMPL_TPL_GET_18(m0, m1, m2, m3, m4, m5, m6, m7, m8, m9, m10, m11, m12, m13, m14, m15, m16, m17, m18, ...) m18
	#define SSVPP_IMPL_TPL_GET_19(m0, m1, m2, m3, m4, m5, m6, m7, m8, m9, m10, m11, m12, m13, m14, m15, m16, m17, m18, m19, ...) m19
	#define SSVPP_IMPL_TPL_GET_20(m0, m1, m2, m3, m4, m5, m6, m7, m8, m9, m10, m11, m12, m13, m14, m15, m16, m17, m18, m19, m20, ...) m20

	#define SSVPP_IMPL_TPL_GET(mIdx, mTpl)	SSVPP_TKNCAT_2(SSVPP_IMPL_TPL_GET_, mIdx) mTpl
	#define SSVPP_TPL_GET(mIdx, mTpl)		SSVPP_IMPL_TPL_GET(mIdx, mTpl)

	#define SSVPP_IMPL_TPL_MAKE(...)		(__VA_ARGS__)
	#define SSVPP_TPL_MAKE(...)				SSVPP_IMPL_TPL_MAKE(__VA_ARGS__)

	#define SSVPP_IMPL_TPL_EXPLODE(...)		__VA_ARGS__
	#define SSVPP_TPL_EXPLODE(mTpl)			SSVPP_EXPAND(SSVPP_IMPL_TPL_EXPLODE mTpl)



	#define SSVPP_IMPL_TPL_CAT_0()				SSVPP_EMPTY()
	#define SSVPP_IMPL_TPL_CAT_1(m0)			m0
	#define SSVPP_IMPL_TPL_CAT_2(m0, m1)		SSVPP_TPL_MAKE(SSVPP_TPL_EXPLODE(m0), SSVPP_TPL_EXPLODE(m1))
	#define SSVPP_IMPL_TPL_CAT_3(m0, m1, ...)	SSVPP_IMPL_TPL_CAT_2(SSVPP_TPL_MAKE(SSVPP_TPL_EXPLODE(m0), SSVPP_TPL_EXPLODE(m1)), __VA_ARGS__)
	#define SSVPP_IMPL_TPL_CAT_4(m0, m1, ...)	SSVPP_IMPL_TPL_CAT_3(SSVPP_TPL_MAKE(SSVPP_TPL_EXPLODE(m0), SSVPP_TPL_EXPLODE(m1)), __VA_ARGS__)
	#define SSVPP_IMPL_TPL_CAT_5(m0, m1, ...)	SSVPP_IMPL_TPL_CAT_4(SSVPP_TPL_MAKE(SSVPP_TPL_EXPLODE(m0), SSVPP_TPL_EXPLODE(m1)), __VA_ARGS__)
	#define SSVPP_IMPL_TPL_CAT_6(m0, m1, ...)	SSVPP_IMPL_TPL_CAT_5(SSVPP_TPL_MAKE(SSVPP_TPL_EXPLODE(m0), SSVPP_TPL_EXPLODE(m1)), __VA_ARGS__)
	#define SSVPP_IMPL_TPL_CAT_7(m0, m1, ...)	SSVPP_IMPL_TPL_CAT_6(SSVPP_TPL_MAKE(SSVPP_TPL_EXPLODE(m0), SSVPP_TPL_EXPLODE(m1)), __VA_ARGS__)
	#define SSVPP_IMPL_TPL_CAT_8(m0, m1, ...)	SSVPP_IMPL_TPL_CAT_7(SSVPP_TPL_MAKE(SSVPP_TPL_EXPLODE(m0), SSVPP_TPL_EXPLODE(m1)), __VA_ARGS__)
	#define SSVPP_IMPL_TPL_CAT_9(m0, m1, ...)	SSVPP_IMPL_TPL_CAT_8(SSVPP_TPL_MAKE(SSVPP_TPL_EXPLODE(m0), SSVPP_TPL_EXPLODE(m1)), __VA_ARGS__)
	#define SSVPP_IMPL_TPL_CAT_10(m0, m1, ...)	SSVPP_IMPL_TPL_CAT_9(SSVPP_TPL_MAKE(SSVPP_TPL_EXPLODE(m0), SSVPP_TPL_EXPLODE(m1)), __VA_ARGS__)
	#define SSVPP_IMPL_TPL_CAT_11(m0, m1, ...)	SSVPP_IMPL_TPL_CAT_10(SSVPP_TPL_MAKE(SSVPP_TPL_EXPLODE(m0), SSVPP_TPL_EXPLODE(m1)), __VA_ARGS__)
	#define SSVPP_IMPL_TPL_CAT_12(m0, m1, ...)	SSVPP_IMPL_TPL_CAT_11(SSVPP_TPL_MAKE(SSVPP_TPL_EXPLODE(m0), SSVPP_TPL_EXPLODE(m1)), __VA_ARGS__)
	#define SSVPP_IMPL_TPL_CAT_13(m0, m1, ...)	SSVPP_IMPL_TPL_CAT_12(SSVPP_TPL_MAKE(SSVPP_TPL_EXPLODE(m0), SSVPP_TPL_EXPLODE(m1)), __VA_ARGS__)
	#define SSVPP_IMPL_TPL_CAT_14(m0, m1, ...)	SSVPP_IMPL_TPL_CAT_13(SSVPP_TPL_MAKE(SSVPP_TPL_EXPLODE(m0), SSVPP_TPL_EXPLODE(m1)), __VA_ARGS__)
	#define SSVPP_IMPL_TPL_CAT_15(m0, m1, ...)	SSVPP_IMPL_TPL_CAT_14(SSVPP_TPL_MAKE(SSVPP_TPL_EXPLODE(m0), SSVPP_TPL_EXPLODE(m1)), __VA_ARGS__)
	#define SSVPP_IMPL_TPL_CAT_16(m0, m1, ...)	SSVPP_IMPL_TPL_CAT_15(SSVPP_TPL_MAKE(SSVPP_TPL_EXPLODE(m0), SSVPP_TPL_EXPLODE(m1)), __VA_ARGS__)
	#define SSVPP_IMPL_TPL_CAT_17(m0, m1, ...)	SSVPP_IMPL_TPL_CAT_16(SSVPP_TPL_MAKE(SSVPP_TPL_EXPLODE(m0), SSVPP_TPL_EXPLODE(m1)), __VA_ARGS__)
	#define SSVPP_IMPL_TPL_CAT_18(m0, m1, ...)	SSVPP_IMPL_TPL_CAT_17(SSVPP_TPL_MAKE(SSVPP_TPL_EXPLODE(m0), SSVPP_TPL_EXPLODE(m1)), __VA_ARGS__)
	#define SSVPP_IMPL_TPL_CAT_19(m0, m1, ...)	SSVPP_IMPL_TPL_CAT_18(SSVPP_TPL_MAKE(SSVPP_TPL_EXPLODE(m0), SSVPP_TPL_EXPLODE(m1)), __VA_ARGS__)
	#define SSVPP_IMPL_TPL_CAT_20(m0, m1, ...)	SSVPP_IMPL_TPL_CAT_19(SSVPP_TPL_MAKE(SSVPP_TPL_EXPLODE(m0), SSVPP_TPL_EXPLODE(m1)), __VA_ARGS__)
	#define SSVPP_IMPL_TPL_CAT_21(m0, m1, ...)	SSVPP_IMPL_TPL_CAT_20(SSVPP_TPL_MAKE(SSVPP_TPL_EXPLODE(m0), SSVPP_TPL_EXPLODE(m1)), __VA_ARGS__)
	#define SSVPP_IMPL_TPL_CAT_22(m0, m1, ...)	SSVPP_IMPL_TPL_CAT_21(SSVPP_TPL_MAKE(SSVPP_TPL_EXPLODE(m0), SSVPP_TPL_EXPLODE(m1)), __VA_ARGS__)
	#define SSVPP_IMPL_TPL_CAT_23(m0, m1, ...)	SSVPP_IMPL_TPL_CAT_22(SSVPP_TPL_MAKE(SSVPP_TPL_EXPLODE(m0), SSVPP_TPL_EXPLODE(m1)), __VA_ARGS__)
	#define SSVPP_IMPL_TPL_CAT_24(m0, m1, ...)	SSVPP_IMPL_TPL_CAT_23(SSVPP_TPL_MAKE(SSVPP_TPL_EXPLODE(m0), SSVPP_TPL_EXPLODE(m1)), __VA_ARGS__)
	#define SSVPP_IMPL_TPL_CAT_25(m0, m1, ...)	SSVPP_IMPL_TPL_CAT_24(SSVPP_TPL_MAKE(SSVPP_TPL_EXPLODE(m0), SSVPP_TPL_EXPLODE(m1)), __VA_ARGS__)
	#define SSVPP_IMPL_TPL_CAT_26(m0, m1, ...)	SSVPP_IMPL_TPL_CAT_25(SSVPP_TPL_MAKE(SSVPP_TPL_EXPLODE(m0), SSVPP_TPL_EXPLODE(m1)), __VA_ARGS__)
	#define SSVPP_IMPL_TPL_CAT_27(m0, m1, ...)	SSVPP_IMPL_TPL_CAT_26(SSVPP_TPL_MAKE(SSVPP_TPL_EXPLODE(m0), SSVPP_TPL_EXPLODE(m1)), __VA_ARGS__)
	#define SSVPP_IMPL_TPL_CAT_28(m0, m1, ...)	SSVPP_IMPL_TPL_CAT_27(SSVPP_TPL_MAKE(SSVPP_TPL_EXPLODE(m0), SSVPP_TPL_EXPLODE(m1)), __VA_ARGS__)
	#define SSVPP_IMPL_TPL_CAT_29(m0, m1, ...)	SSVPP_IMPL_TPL_CAT_28(SSVPP_TPL_MAKE(SSVPP_TPL_EXPLODE(m0), SSVPP_TPL_EXPLODE(m1)), __VA_ARGS__)
	#define SSVPP_IMPL_TPL_CAT_30(m0, m1, ...)	SSVPP_IMPL_TPL_CAT_29(SSVPP_TPL_MAKE(SSVPP_TPL_EXPLODE(m0), SSVPP_TPL_EXPLODE(m1)), __VA_ARGS__)
	#define SSVPP_IMPL_TPL_CAT_31(m0, m1, ...)	SSVPP_IMPL_TPL_CAT_30(SSVPP_TPL_MAKE(SSVPP_TPL_EXPLODE(m0), SSVPP_TPL_EXPLODE(m1)), __VA_ARGS__)
	#define SSVPP_IMPL_TPL_CAT_32(m0, m1, ...)	SSVPP_IMPL_TPL_CAT_31(SSVPP_TPL_MAKE(SSVPP_TPL_EXPLODE(m0), SSVPP_TPL_EXPLODE(m1)), __VA_ARGS__)
	#define SSVPP_IMPL_TPL_CAT_33(m0, m1, ...)	SSVPP_IMPL_TPL_CAT_32(SSVPP_TPL_MAKE(SSVPP_TPL_EXPLODE(m0), SSVPP_TPL_EXPLODE(m1)), __VA_ARGS__)
	#define SSVPP_IMPL_TPL_CAT_34(m0, m1, ...)	SSVPP_IMPL_TPL_CAT_33(SSVPP_TPL_MAKE(SSVPP_TPL_EXPLODE(m0), SSVPP_TPL_EXPLODE(m1)), __VA_ARGS__)
	#define SSVPP_IMPL_TPL_CAT_35(m0, m1, ...)	SSVPP_IMPL_TPL_CAT_34(SSVPP_TPL_MAKE(SSVPP_TPL_EXPLODE(m0), SSVPP_TPL_EXPLODE(m1)), __VA_ARGS__)
	#define SSVPP_IMPL_TPL_CAT_36(m0, m1, ...)	SSVPP_IMPL_TPL_CAT_35(SSVPP_TPL_MAKE(SSVPP_TPL_EXPLODE(m0), SSVPP_TPL_EXPLODE(m1)), __VA_ARGS__)
	#define SSVPP_IMPL_TPL_CAT_37(m0, m1, ...)	SSVPP_IMPL_TPL_CAT_36(SSVPP_TPL_MAKE(SSVPP_TPL_EXPLODE(m0), SSVPP_TPL_EXPLODE(m1)), __VA_ARGS__)
	#define SSVPP_IMPL_TPL_CAT_38(m0, m1, ...)	SSVPP_IMPL_TPL_CAT_37(SSVPP_TPL_MAKE(SSVPP_TPL_EXPLODE(m0), SSVPP_TPL_EXPLODE(m1)), __VA_ARGS__)
	#define SSVPP_IMPL_TPL_CAT_39(m0, m1, ...)	SSVPP_IMPL_TPL_CAT_38(SSVPP_TPL_MAKE(SSVPP_TPL_EXPLODE(m0), SSVPP_TPL_EXPLODE(m1)), __VA_ARGS__)
	#define SSVPP_IMPL_TPL_CAT_40(m0, m1, ...)	SSVPP_IMPL_TPL_CAT_39(SSVPP_TPL_MAKE(SSVPP_TPL_EXPLODE(m0), SSVPP_TPL_EXPLODE(m1)), __VA_ARGS__)

	#define SSVPP_IMPL_TPL_CAT(...)	SSVPP_TKNCAT_2(SSVPP_IMPL_TPL_CAT_, SSVPP_VA_NUM_ARGS(__VA_ARGS__)) (__VA_ARGS__)
	#define SSVPP_TPL_CAT(...)		SSVPP_IMPL_TPL_CAT(__VA_ARGS__)

	#define SSVPP_TPL_SIZE(mTpl)	SSVPP_VA_NUM_ARGS(SSVPP_TPL_EXPLODE(mTpl))
}

#endif
