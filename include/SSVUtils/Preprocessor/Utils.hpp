// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_PREPROCESSOR_UTILS
#define SSVU_PREPROCESSOR_UTILS

namespace ssvu
{
	#define SSVPP_EMPTY()
	#define SSVPP_COMMA() ,

	#define SSVPP_STRINGIFY_IMPL(mX)	#mX
	#define SSVPP_STRINGIFY(mX)			SSVPP_STRINGIFY_IMPL(mX)

	#define SSVPP_EXPAND(mX) mX

	#define SSVPP_IMPL_TKNCAT_2(m0, m1)				m0 ## m1
	#define SSVPP_IMPL_TKNCAT_3(m0, m1, m2)			m0 ## m1 ## m2
	#define SSVPP_IMPL_TKNCAT_4(m0, m1, m2, m3)		m0 ## m1 ## m2 ## m3
	#define SSVPP_IMPL_TKNCAT_5(m0, m1, m2, m3, m4)	m0 ## m1 ## m2 ## m3 ## m4

	#define SSVPP_TKNCAT_2(m0, m1)					SSVPP_IMPL_TKNCAT_2(m0, m1)
	#define SSVPP_TKNCAT_3(m0, m1, m2)				SSVPP_IMPL_TKNCAT_3(m0, m1, m2)
	#define SSVPP_TKNCAT_4(m0, m1, m2, m3)			SSVPP_IMPL_TKNCAT_4(m0, m1, m2, m3)
	#define SSVPP_TKNCAT_5(m0, m1, m2, m3, m4)		SSVPP_IMPL_TKNCAT_5(m0, m1, m2, m3, m4)

	//#define SSVPP_IMPL_WHILE(mPredicate, mOp

	/*
		#define SSVPP_IMPL_WHILE_IMPL(pred, op, ...) \
			SSVPP_IF(pred(__VA_ARGS__)) \
			( \
				SSVPP_DEFER(SSVPP_IMPL_WHILE_IMPL) () \
				( \
					pred, op, op(__VA_ARGS__) \
				), \
				__VA_ARGS__ \
			)

		#define SSVPP_WHILE(...) SSVPP_IMPL_WHILE_IMPL(__VA_ARGS__)
	*/
}

#endif
