// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_CORE_COMMON_LIKELYUNLIKELY
#define SSVU_CORE_COMMON_LIKELYUNLIKELY

#if (defined(SSVU_COMPILER_CLANG) || defined(SSVU_COMPILER_GCC))
	/// @macro Micro-optimization telling the compiler that this condition is more likely to happen than the `else` branch.
	#define SSVU_LIKELY(mCondition) __builtin_expect(!!(mCondition), 1)

	/// @macro Micro-optimization telling the compiler that this condition is less likely to happen than the `else` branch.
	#define SSVU_UNLIKELY(mCondition) __builtin_expect(!!(mCondition), 0)
#else
	#define SSVU_LIKELY(mCondition)		mCondition
	#define SSVU_UNLIKELY(mCondition)	mCondition
#endif

#endif
