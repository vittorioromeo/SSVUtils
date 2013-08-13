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
				bool mustNormalize{true};

				inline void normalize()
				{
					if(!mustNormalize) return;
					mustNormalize = false;
					replaceAll(path, R"(\)", "/");
					replaceAll(path, R"(\\)", "/");
					replaceAll(path, "//", "/");
					if(isFolder() && !endsWith(path, "/")) path += ("/");
				}

			public:
				inline Path() = default;
				inline Path(const char* mPath) : path{mPath} { }
				inline Path(const std::string& mPath) : path{mPath} { }

				inline const std::string& getStr() const	{ const_cast<Path*>(this)->normalize(); return path; }
				inline const char* getCStr() const			{ return getStr().c_str(); }

				inline bool isFolder() const
				{
					struct stat fileStat;
					int err{stat(getCStr(), &fileStat)};
					if(err != 0) return false;
					return (fileStat.st_mode & S_IFMT) == S_IFDIR;
				}

				inline bool hasExtension(const std::string& mExtension) const	{ return endsWith(toLower(getStr()), toLower(mExtension)); }
				inline bool exists() const										{ struct stat buf; return stat(getCStr(), &buf) != -1; }
				inline bool isRootOrParent() const								{ return endsWith(getStr(), "./") || endsWith(getStr(), "../"); }
				inline Path getParent() const
				{
					std::string str(getStr());
					for(auto i(str.size() - 1); i > 0; --i) if(str[i] == '/') return {str.substr(0, i + 1)};
					return {""};
				}

				inline void operator=(const std::string& mPath)			{ mustNormalize = true; path = mPath; }
				inline Path operator+(const std::string& mPath) const	{ return {path + mPath}; }
				inline bool operator<(const Path& mPath) const			{ return getStr() < mPath.getStr(); }

				inline operator const std::string&() const { return getStr(); }
		};

		inline std::ostream& operator <<(std::ostream& mOStream, const Path& mPath) { return mOStream << mPath.getStr(); }
	}
}

#endif
