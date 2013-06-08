// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#include <dirent.h>
#include <sys/stat.h>
#include "SSVUtils/FileSystem/Utils.h"
#include "SSVUtils/Utils/UtilsString.h"
#include "SSVUtils/Log/Log.h"

using namespace std;

namespace ssvu
{
	namespace FileSystem
	{
		bool exists(const string& mPath) { struct stat buf; return stat(getNormalizedPath(mPath).c_str(), &buf) != -1; }
		bool hasExtension(const string& mFileName, const string& mExtension) { return endsWith(toLower(mFileName), toLower(mExtension)); }
		bool isFolder(const string& mPath)
		{
			struct stat fileStat;
			int err{stat(getNormalizedPath(mPath).c_str(), &fileStat)};
			if(err != 0) return false;
			return (fileStat.st_mode & S_IFMT) == S_IFDIR;
		}
		bool isRootOrParent(const string& mPath) { return (endsWith(mPath, ".") || endsWith(mPath, "..")); }
		string getNormalizedPath(const string& mPath)
		{
			string result{mPath};
			while(endsWith(result, R"(\)")) result = result.substr(0, result.size() - 1);
			while(endsWith(result, "/")) result = result.substr(0, result.size() - 1);
			return result;
		}
		string getNameFromPath(const string& mPath, const string& mPrefix, const string& mSuffix)
		{
			return mPath.substr(mPrefix.length(), mPath.length() - mPrefix.length() - mSuffix.length());
		}
		string getFileContents(const string& mPath)
		{
			FILE* fptr{fopen(mPath.c_str(), "rb")};
			fseek(fptr, 0, SEEK_END);
			size_t fsize(ftell(fptr));
			fseek(fptr, 0, SEEK_SET);
			string content; content.resize(fsize);
			if(fread(const_cast<char*>(content.c_str()), 1, fsize, fptr) != fsize) log("Error: " + mPath, "File loading");
			fclose(fptr); return content;
		}
		void createFolder(const string& mPath)
		{
			#ifdef _WIN32
				mkdir(getNormalizedPath(mPath).c_str());
			#else
				mkdir(getNormalizedPath(mPath).c_str(), 0755);
			#endif
		}
	}
}

