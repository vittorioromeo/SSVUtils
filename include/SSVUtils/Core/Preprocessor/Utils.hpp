// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_CORE_PREPROCESSOR_UTILS
#define SSVU_CORE_PREPROCESSOR_UTILS

#define SSVPP_EMPTY()
#define SSVPP_COMMA()		,
#define SSVPP_EXPAND(mX)	mX

#define SSVPP_RAW(...)	__VA_ARGS__
#define __R(...)		SSVPP_RAW(__VA_ARGS__)

#define SSVPP_TOSTR_IMPL(mX)	#mX
#define SSVPP_TOSTR(mX)			SSVPP_TOSTR_IMPL(mX)

#define SSVPP_IMPL_TKNCAT_2(m0, m1)				m0 ## m1
#define SSVPP_IMPL_TKNCAT_3(m0, m1, m2)			m0 ## m1 ## m2
#define SSVPP_IMPL_TKNCAT_4(m0, m1, m2, m3)		m0 ## m1 ## m2 ## m3
#define SSVPP_IMPL_TKNCAT_5(m0, m1, m2, m3, m4)	m0 ## m1 ## m2 ## m3 ## m4

#define SSVPP_TKNCAT_2(m0, m1)					SSVPP_IMPL_TKNCAT_2(m0, m1)
#define SSVPP_TKNCAT_3(m0, m1, m2)				SSVPP_IMPL_TKNCAT_3(m0, m1, m2)
#define SSVPP_TKNCAT_4(m0, m1, m2, m3)			SSVPP_IMPL_TKNCAT_4(m0, m1, m2, m3)
#define SSVPP_TKNCAT_5(m0, m1, m2, m3, m4)		SSVPP_IMPL_TKNCAT_5(m0, m1, m2, m3, m4)

#endif
