// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_FILESYSTEM_PATH
#define SSVU_FILESYSTEM_PATH

#include <cstdlib>
#include <dirent.h>
#include <sys/stat.h>
#include <iostream>
#include <string>
#include "SSVUtils/FileSystem/Enums.h"
#include "SSVUtils/FileSystem/Utils.h"
#include "SSVUtils/String/Utils.h"

namespace ssvu
{
	namespace FileSystem
	{
		class Path
		{
			private:
				std::string path;
				bool needsNormalization{true};

				inline bool isFolder()
				{
					struct stat fileStat;
					int err{stat(path.c_str(), &fileStat)};
					if(err != 0) return false;
					return (fileStat.st_mode & S_IFMT) == S_IFDIR;
				}

				inline void normalize()
				{
					replaceAll(path, R"(\)", "/");
					replaceAll(path, R"(\\)", "/");
					replaceAll(path, "//", "/");
					if(isFolder() && !endsWith(path, "/")) path += ("/");
				}

				inline void normalizeIfNeeded()
				{
					if(!needsNormalization) return;
					normalize(); needsNormalization = false;
				}

			public:
				inline Path(const char* mPath) : path{mPath} { }
				inline Path(const std::string& mPath) : path{mPath} { }

				inline const std::string& getStr() const	{ const_cast<Path*>(this)->normalizeIfNeeded(); return path; }
				inline const char* getCStr() const			{ const_cast<Path*>(this)->normalizeIfNeeded(); return path.c_str(); }

				inline void operator=(const std::string& mPath)			{ needsNormalization = true; path = mPath; }
				inline Path operator+(const std::string& mPath) const	{ return Path{path + mPath}; }
				inline bool operator<(const Path& mPath) const			{ return path < mPath.getStr(); }

				inline operator const std::string&() const { return getStr(); }
		};

		inline std::ostream& operator <<(std::ostream& mOStream, const Path& mPath) { return mOStream << mPath.getStr(); }
	}
}

#endif
