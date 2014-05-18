// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_CORE_PREPROCESSOR_ARGCOUNT
#define SSVU_CORE_PREPROCESSOR_ARGCOUNT

#define SSVPP_IMPL_NSEQ(	m1, m2, m3, m4, m5, m6, m7, m8, m9, m10, m11, m12, m13, m14, m15, m16, m17, m18, m19, m20, m21, m22, m23, m24, m25, m26, m27, m28, m29, m30, m31, m32, m33, m34, m35, m36, m37, m38, m39, m40, m41, m42, m43, m44, m45, m46, m47, m48, m49, m50, m51, m52, m53, m54, m55, m56, m57, m58, m59, m60, m61, m62, m63, mN, ...) mN
#define SSVPP_IMPL_RSEQ()	63, 62, 61, 60, 59, 58, 57, 56, 55, 54,  53,  52,  51,  50,  49,  48,  47,  46,  45,  44,  43,  42,  41,  40,  39,  38,  37,  36,  35,  34,  33,  32,  31,  30,  29,  28,  27,  26,  25,  24,  23,  22,  21,  20,  19,  18,  17,  16,  15,  14,  13,  12,  11,  10,  9,   8,   7,   6,   5,   4,   3,   2,   1,   0
#define SSVPP_IMPL_CSEQ()	1,  1,  1,  1,  1,  1,  1,  1,  1,  1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   0,   0

#define SSVPP_IMPL_N_ARG(...)			SSVPP_IMPL_NSEQ(__VA_ARGS__)

#define SSVPP_IMPL_NARGS_COMMA(...)		,
#define SSVPP_IMPL_NARGS_HASCOMMA(...)	SSVPP_IMPL_N_ARG(__VA_ARGS__, SSVPP_IMPL_CSEQ())

#define SSVPP_IMPL_NARGS_DISPATCH_3_01(mN)	0
#define SSVPP_IMPL_NARGS_DISPATCH_3_00(mN)	1
#define SSVPP_IMPL_NARGS_DISPATCH_3_11(mN)	mN

#define SSVPP_IMPL_NARGS_DISPATCH_2(mA, mB, mN)	SSVPP_IMPL_NARGS_DISPATCH_3_ ## mA ## mB(mN)
#define SSVPP_IMPL_NARGS_DISPATCH_1(mA, mB, mN)	SSVPP_IMPL_NARGS_DISPATCH_2(mA, mB, mN)

#define SSVPP_IMPL_ARGCOUNT(...)	SSVPP_IMPL_NARGS_DISPATCH_1(SSVPP_IMPL_NARGS_HASCOMMA(__VA_ARGS__), SSVPP_IMPL_NARGS_HASCOMMA(SSVPP_IMPL_NARGS_COMMA __VA_ARGS__ ()), SSVPP_IMPL_N_ARG(__VA_ARGS__, SSVPP_IMPL_RSEQ()))
#define SSVPP_ARGCOUNT(...)			SSVPP_IMPL_ARGCOUNT(__VA_ARGS__)

#endif
