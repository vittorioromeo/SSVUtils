// Copyright (c) 2013-2015 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_CORE_PREPROCESSOR_EVAL
#define SSVU_CORE_PREPROCESSOR_EVAL

#define SSVPP_IMPL_EVAL(...) SSVPP_IMPL_EVAL_0(__VA_ARGS__)

/// @macro Forces a large number of preprocessor rescan passes.
#define SSVPP_EVAL(...) SSVPP_IMPL_EVAL(__VA_ARGS__)

#endif
