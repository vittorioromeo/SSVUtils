// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#include <cstdlib>
#include <dirent.h>
#include <sys/stat.h>
#include "SSVUtils/FileSystem/Utils.h"
#include "SSVUtils/String/Utils.h"
#include "SSVUtils/Log/Log.h"

using namespace std;

namespace ssvu
{
	namespace FileSystem
	{
		namespace Internal
		{
			bool isFolderNoNormalize(const string& mPath)
			{
				struct stat fileStat;
				int err{stat(mPath.c_str(), &fileStat)};
				if(err != 0) return false;
				return (fileStat.st_mode & S_IFMT) == S_IFDIR;
			}
		}

		bool exists(const string& mPath) { struct stat buf; return stat(getNormalizedPath(mPath).c_str(), &buf) != -1; }
		bool hasExtension(const string& mFileName, const string& mExtension) { return endsWith(toLower(mFileName), toLower(mExtension)); }
		bool isFolder(const string& mPath) { return Internal::isFolderNoNormalize(getNormalizedPath(mPath)); }
		bool isRootOrParent(const string& mPath) { return endsWith(mPath, "./") || endsWith(mPath, "../"); }
		void normalizePath(string& mPath)
		{
			replaceAll(mPath, R"(\)", "/");
			replaceAll(mPath, R"(\\)", "/");
			replaceAll(mPath, "//", "/");
			if(Internal::isFolderNoNormalize(mPath) && !endsWith(mPath, "/")) mPath.append("/");
		}
		string getNormalizedPath(string mPath) { normalizePath(mPath); return mPath; }
		string getParentPath(string mPath)
		{
			normalizePath(mPath);
			for(auto i(mPath.length() - 1); i > 0; --i)
				if(mPath[i] == '/') return mPath.substr(0, i + 1);

			return "";
		}
		string getNameFromPath(const string& mPath, const string& mPrefix, const string& mSuffix)
		{
			return getNormalizedPath(mPath).substr(mPrefix.length(), mPath.length() - mPrefix.length() - mSuffix.length());
		}
		string getFileContents(const string& mPath)
		{
			FILE* fptr{fopen(mPath.c_str(), "rb")};
			fseek(fptr, 0, SEEK_END);
			size_t fsize(ftell(fptr));
			fseek(fptr, 0, SEEK_SET);
			string content; content.resize(fsize);
			if(fread(const_cast<char*>(content.c_str()), 1, fsize, fptr) != fsize) log("Error: " + mPath, "ssvu::FileSystem::getFileContents");
			fclose(fptr); return content;
		}
		void createFolder(string mPath)
		{
			normalizePath(mPath);

			#ifdef _WIN32
				mkdir(mPath.c_str());
			#else
				mkdir(mPath.c_str(), 0755);
			#endif
		}
		void removeFile(string mPath)
		{
			normalizePath(mPath);
			if(remove(mPath.c_str()) != 0) log("Error removing file: " + mPath, "ssvu::FileSystem::removeFile");
		}

		void expandUserPath(string& mPath)
		{
			// TODO: WIN32 NEEDS TESTING!

			normalizePath(mPath);
			string userHome;

			#ifdef _WIN32
				if(getenv("HOME") != NULL) userHome = getenv("HOME");
				else if(getenv("USERPROFILE") != NULL) userHome = getenv("USERPROFILE");
				else if(getenv("HOMEPATH") == NULL) return;
				else
				{
					string drive{getenv("HOMEDRIVE")};
					userHome = drive + getenv("HOMEPATH");
				}
			#else
				if(getenv("HOME") != NULL) userHome = getenv("HOME");
			#endif

			replaceAll(mPath, "~", userHome);
			normalizePath(mPath);
		}

		string getExpandedUserPath(string mPath) { expandUserPath(mPath); return mPath; }
	}
}

