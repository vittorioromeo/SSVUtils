// Copyright (c) 2013-2015 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_CORE_PREPROCESSOR_UTILS
#define SSVU_CORE_PREPROCESSOR_UTILS

#define SSVPP_EMPTY()
#define SSVPP_DOT() .
#define SSVPP_COMMA() ,
#define SSVPP_DEFER(mX) mX SSVPP_EMPTY()
#define SSVPP_OBSTRUCT(...) __VA_ARGS__ SSVPP_DEFER(SSVPP_EMPTY)()
#define SSVPP_EXPAND(...) __VA_ARGS__
#define SSVPP_EAT(...)

#define SSVPP_RAW(...)	__VA_ARGS__
#define __R(...)		SSVPP_RAW(__VA_ARGS__)

#define SSVPP_TOSTR_IMPL(...)	#__VA_ARGS__
#define SSVPP_TOSTR(...)		SSVPP_TOSTR_IMPL(__VA_ARGS__)

#define SSVPP_IMPL_TKNCAT_2(m0, m1)				m0 ## m1
#define SSVPP_IMPL_TKNCAT_3(m0, m1, m2)			m0 ## m1 ## m2
#define SSVPP_IMPL_TKNCAT_4(m0, m1, m2, m3)		m0 ## m1 ## m2 ## m3
#define SSVPP_IMPL_TKNCAT_5(m0, m1, m2, m3, m4)	m0 ## m1 ## m2 ## m3 ## m4

#define SSVPP_TKNCAT_2(m0, m1)					SSVPP_IMPL_TKNCAT_2(m0, m1)
#define SSVPP_TKNCAT_3(m0, m1, m2)				SSVPP_IMPL_TKNCAT_3(m0, m1, m2)
#define SSVPP_TKNCAT_4(m0, m1, m2, m3)			SSVPP_IMPL_TKNCAT_4(m0, m1, m2, m3)
#define SSVPP_TKNCAT_5(m0, m1, m2, m3, m4)		SSVPP_IMPL_TKNCAT_5(m0, m1, m2, m3, m4)

// TODO: move, generate
#define SSVPP_IMPL_EVAL_5(...) __VA_ARGS__
#define SSVPP_IMPL_EVAL_4(...) SSVPP_IMPL_EVAL_5(SSVPP_IMPL_EVAL_5(SSVPP_IMPL_EVAL_5(__VA_ARGS__)))
#define SSVPP_IMPL_EVAL_3(...) SSVPP_IMPL_EVAL_4(SSVPP_IMPL_EVAL_4(SSVPP_IMPL_EVAL_4(__VA_ARGS__)))
#define SSVPP_IMPL_EVAL_2(...) SSVPP_IMPL_EVAL_3(SSVPP_IMPL_EVAL_3(SSVPP_IMPL_EVAL_3(__VA_ARGS__)))
#define SSVPP_IMPL_EVAL_1(...) SSVPP_IMPL_EVAL_2(SSVPP_IMPL_EVAL_2(SSVPP_IMPL_EVAL_2(__VA_ARGS__)))
#define SSVPP_IMPL_EVAL_0(...) SSVPP_IMPL_EVAL_1(SSVPP_IMPL_EVAL_1(SSVPP_IMPL_EVAL_1(__VA_ARGS__)))

#define SSVPP_IMPL_EVAL(...) SSVPP_IMPL_EVAL_0(__VA_ARGS__)
#define SSVPP_EVAL(...) SSVPP_IMPL_EVAL(__VA_ARGS__)

#endif
