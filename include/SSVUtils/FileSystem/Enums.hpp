// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_FILESYSTEM_ENUMS
#define SSVU_FILESYSTEM_ENUMS

namespace ssvu
{
	namespace FileSystem
	{
		enum class Mode{Recurse, Single};
		enum class Type{All, File, Folder};
		enum class Pick{Any, ByExt, ByName};
		enum class Sort{Alphabetic, Unsorted};
	}
}

#endif
