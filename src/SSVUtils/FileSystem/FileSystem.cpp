// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#include <dirent.h>
#include <sys/stat.h>
#include "SSVUtils/FileSystem/FileSystem.h"
#include "SSVUtils/Log/Log.h"
#include "SSVUtils/Utils/UtilsString.h"

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
		void traverse(const string& mPath, function<void(string, string)> mFunction)
		{
			if(isFolder(mPath))
			{
				DIR *dir{opendir(mPath.c_str())};
				struct dirent *entry{readdir(dir)};

				while(entry != NULL)
				{
					string name{entry->d_name}, path{getNormalizedPath(mPath) + "/" + name};
					mFunction(name, path);
					entry = readdir(dir);
				}

				closedir(dir);
			}
			else log("Directory \"" + mPath + "\" not found", "FileSystem");
		}

		void recursiveFillFiles(vector<string>& mResult, const string& mPath)
		{
			traverse(mPath, [&](const string&, const string& path)
			{
				if(!isRootOrParent(path))
				{
					if(isFolder(path)) recursiveFillFiles(mResult, path);
					else if(!isFolder(path)) mResult.push_back(path);
				}
			});
		}
		void recursiveFillFilesByExtension(vector<string>& mResult, const string& mPath, const string& mExtension)
		{
			traverse(mPath, [&](const string& name, const string& path)
			{
				if(!isRootOrParent(path))
				{
					if(isFolder(path)) recursiveFillFiles(mResult, path);
					else if(!isFolder(path)) if(endsWith(name, mExtension)) mResult.push_back(path);
				}
			});
		}
		void recursiveFillFolders(vector<string>& mResult, const string& mPath)
		{
			traverse(mPath, [&](const string&, const string& path)
			{
				if(!isRootOrParent(path)) if(isFolder(path)) { recursiveFillFolders(mResult, path); mResult.push_back(path); }
			});
		}
		void recursiveFillAll(vector<string>& mResult, const string& mPath)
		{
			traverse(mPath, [&](const string&, const string& path)
			{
				if(!isRootOrParent(path))
				{
					if(isFolder(path)) recursiveFillAll(mResult, path);
					mResult.push_back(path);
				}
			});
		}

		vector<string> getFiles(const string& mPath)
		{
			vector<string> result;
			traverse(mPath, [&](const string&, const string& path)
			{
				if(!isRootOrParent(path)) if(!isFolder(path)) result.push_back(path);
			});
			return result;
		}
		vector<string> getFilesByExtension(const string& mPath, const string& mExtension)
		{
			vector<string> result;
			traverse(mPath, [&](const string& name, const string& path)
			{
				if(!isRootOrParent(path)) if(!isFolder(path)) if(endsWith(name, mExtension)) result.push_back(path);
			});
			return result;
		}
		vector<string> getFolders(const string& mPath)
		{
			vector<string> result;
			traverse(mPath, [&](const string&, const string& path)
			{
				if(!isRootOrParent(path)) if(isFolder(path)) result.push_back(path);
			});
			return result;
		}
		vector<string> getAll(const string& mPath)
		{
			vector<string> result;
			traverse(mPath, [&](const string&, const string& path)
			{
				if(!isRootOrParent(path)) result.push_back(path);
			});
			return result;
		}
		vector<string> getRecursiveFiles(const string& mPath) { vector<string> result; recursiveFillFiles(result, mPath); return result; }
		vector<string> getRecursiveFilesByExtension(const string& mPath, const string& mExtension) { vector<string> result; recursiveFillFilesByExtension(result, mPath, mExtension); return result; }
		vector<string> getRecursiveFolders(const string& mPath) { vector<string> result; recursiveFillFolders(result, mPath); return result; }
		vector<string> getRecursiveAll(const string& mPath) { vector<string> result; recursiveFillAll(result, mPath); return result; }

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
	}
}

