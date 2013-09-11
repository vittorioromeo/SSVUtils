// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_FILESYSTEM_PATH
#define SSVU_FILESYSTEM_PATH

#include <cassert>
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
					if(!endsWith(path, "/") && isFolder()) path += ("/");
				}

			public:
				inline Path() = default;
				inline Path(const char* mPath) : path{mPath} { }
				inline Path(const std::string& mPath) : path{mPath} { }

				inline const std::string& getStr() const	{ const_cast<Path*>(this)->normalize(); return path; }
				inline const char* getCStr() const noexcept	{ return getStr().c_str(); }

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
					auto str(getStr());
					for(auto i(str.size() - 1); i > 0; --i) if(str[i] == '/') return {str.substr(0, i + 1)};
					return {""};
				}
				inline std::string getFileName() const
				{
					assert(!isFolder());

					auto str(getStr());
					auto nameBegin(str.find_last_of('/') + 1);
					return str.substr(nameBegin, str.size() - nameBegin);
				}
				inline std::string getFileNameNoExtensions() const
				{
					auto str(getFileName());
					auto extBegin(str.find_first_of('.', beginsWith(str, '.') ? 1 : 0));
					return str.substr(0, extBegin);
				}
				inline std::string getFolderName() const
				{
					assert(isFolder());

					std::string str(getStr());
					assert(endsWith(str, '/'));

					str.erase(std::end(str) - 1);
					auto nameBegin(str.find_last_of('/') + 1);
					return str.substr(nameBegin, str.size() - nameBegin);
				}
				inline std::string getExtension() const
				{
					auto str(getFileName()); if(beginsWith(str, '.')) str.erase(std::begin(str));
					auto extBegin(str.find_last_of('.'));
					return extBegin == std::string::npos ? "" : str.substr(extBegin, str.size() - extBegin);
				}
				inline std::string getAllExtensions() const
				{
					auto str(getFileName());
					auto extBegin(str.find_first_of('.', beginsWith(str, '.') ? 1 : 0));
					return extBegin == std::string::npos ? "" : str.substr(extBegin, str.size() - extBegin);
				}

				inline Path operator+(const std::string& mPath) const	{ return {path + mPath}; }
				inline bool operator<(const Path& mPath) const			{ return getStr() < mPath.getStr(); }

				inline operator const std::string&() const { return getStr(); }
		};

		inline std::ostream& operator<<(std::ostream& mOStream, const Path& mPath) { return mOStream << mPath.getStr(); }
	}
}

SSVU_TEST("Path tests")
{
	using namespace std;
	using namespace ssvu;
	using namespace ssvu::FileSystem;

	Path path{"/usr"};
	EXPECT(path.getStr() == "/usr/");
	EXPECT(path.getFolderName() == "usr");

	path = "/usr.txt";
	EXPECT(path.getStr() == "/usr.txt");
	//EXPECT(path.getFolderName() == ""); TODO: throw an exception if isFolder() == false?

	path = "/usr.txt/banana/.log";
	EXPECT(path.getStr() == "/usr.txt/banana/.log");
	EXPECT(path.getParent().getFolderName() == "banana");
	EXPECT(path.getExtension() == "");
	EXPECT(path.getAllExtensions() == "");
	EXPECT(path.getFileName() == ".log");
	EXPECT(path.getFileNameNoExtensions() == ".log");

	path = "///////////usr//////";
	EXPECT(path.getStr() == "/usr/");
	EXPECT(path.getFolderName() == "usr");

	path = "/////\/\/\///\\\/usr///\\\///test//test2/////test4.png";
	EXPECT(path.getStr() == "/usr/test/test2/test4.png");
	EXPECT(path.getExtension() == ".png");
	EXPECT(path.getAllExtensions() == ".png");
	EXPECT(path.getFileName() == "test4.png");
	EXPECT(path.getFileNameNoExtensions() == "test4");

	path = "/////\/\/\///\\\/usr///\\\///test//test2/////test4.png.bak";
	EXPECT(path.getStr() == "/usr/test/test2/test4.png.bak");
	EXPECT(path.getExtension() == ".bak");
	EXPECT(path.getAllExtensions() == ".png.bak");
	EXPECT(path.getFileName() == "test4.png.bak");
	EXPECT(path.getFileNameNoExtensions() == "test4");
}
SSVU_TEST_END();

#endif
