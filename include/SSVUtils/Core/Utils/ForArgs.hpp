// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_CORE_UTILS_FORARGS
#define SSVU_CORE_UTILS_FORARGS

namespace ssvu
{
	/// @brief Calls `mFn` for each argument passed to the function.
	template<typename TF, typename... TArgs> inline auto forArgs(TF&& mFn, TArgs&&... mArgs)
	{
		return (void) std::initializer_list<int>{0, (std::ref(mFn)(FWD(mArgs)), 0)...}, FWD(mFn);
	}
}

#endif
