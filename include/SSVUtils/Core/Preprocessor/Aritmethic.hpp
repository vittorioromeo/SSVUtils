// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_CORE_PREPROCESSOR_ARITHMETIC
#define SSVU_CORE_PREPROCESSOR_ARITHMETIC

#define SSVPP_IMPL_DECREMENT(mX)	SSVPP_IMPL_DECREMENT_ ## mX
#define SSVPP_DECREMENT(mX)			SSVPP_IMPL_DECREMENT(mX)

#define SSVPP_IMPL_INCREMENT(mX)	SSVPP_IMPL_INCREMENT_ ## mX
#define SSVPP_INCREMENT(mX)			SSVPP_IMPL_INCREMENT(mX)

#endif
