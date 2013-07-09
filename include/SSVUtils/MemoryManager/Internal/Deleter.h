// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_MEMORYMANAGER_INTERNAL
#define SSVU_MEMORYMANAGER_INTERNAL

#include <vector>
#include "SSVUtils/Global/Typedefs.h"

namespace ssvu
{
	namespace Internal
	{
		template<typename T> struct Deleter { inline bool operator()(const Uptr<T>& mItem) const { return !mItem->isAlive(); } };
	}
}

#endif
