// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_CORE_FILESYSTEM
#define SSVU_CORE_FILESYSTEM

namespace ssvu
{
	namespace FileSystem
	{
		/// @brief Type shortcut for `struct stat`.
		using CStat = struct stat;
	}
}

#include "SSVUtils/Core/FileSystem/Enums.hpp"
#include "SSVUtils/Core/FileSystem/Path.hpp"
#include "SSVUtils/Core/FileSystem/Utils.hpp"
#include "SSVUtils/Core/FileSystem/Scan.hpp"

/// @brief `ssvufs` is a namespace alias for `ssvu::FileSystem`.
namespace ssvufs = ssvu::FileSystem;

#endif
