// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_CORE_FILESYSTEM_ENUMS
#define SSVU_CORE_FILESYSTEM_ENUMS

namespace ssvu
{
	namespace FileSystem
	{
		/// @brief Filesystem scan mode.
		enum class Mode{Recurse, Single};

		/// @brief Filesystem scan type.
		enum class Type{All, File, Folder};

		/// @brief Filesystem scan filtering.
		enum class Pick{Any, ByExt, ByName};

		/// @brief Filesystem scan sorting.
		enum class Sort{Alphabetic, Unsorted};
	}
}

#endif
