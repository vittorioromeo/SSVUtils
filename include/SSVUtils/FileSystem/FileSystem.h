// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_FILESYSTEM
#define SSVU_FILESYSTEM

#include <vector>
#include <string>
#include <functional>

namespace ssvu
{
	namespace FileSystem
	{
		// Helpers
		bool exists(const std::string& mPath);
		bool hasExtension(const std::string& mFileName, const std::string& mExtension);
		bool isFolder(const std::string& mPath);
		bool isRootOrParent(const std::string& mPath);
		std::string getNormalizedPath(const std::string& mPath);
		std::string getNameFromPath(const std::string& mPath, const std::string& mPrefix, const std::string& mSuffix);
		void traverse(const std::string& mPath, std::function<void(std::string, std::string)> mFunction);

		// Fills (they fill an existing vector)
		void recursiveFillFiles(std::vector<std::string>& mResult, const std::string& mPath);
		void recursiveFillFilesByExtension(std::vector<std::string>& mResult, const std::string& mPath, const std::string& mExtension);
		void recursiveFillFolders(std::vector<std::string>& mResult, const std::string& mPath);
		void recursiveFillAll(std::vector<std::string>& mResult, const std::string& mPath);

		// Public interface
		std::vector<std::string> getFiles(const std::string& mPath);
		std::vector<std::string> getFilesByExtension(const std::string& mPath, const std::string& mExtension);
		std::vector<std::string> getFolders(const std::string& mPath);
		std::vector<std::string> getAll(const std::string& mPath);
		std::vector<std::string> getRecursiveFiles(const std::string& mPath);
		std::vector<std::string> getRecursiveFilesByExtension(const std::string& mPath, const std::string& mExtension);
		std::vector<std::string> getRecursiveFolders(const std::string& mPath);
		std::vector<std::string> getRecursiveAll(const std::string& mPath);

		// Utilities
		std::string getFileContents(const std::string& mPath);
	}

	// TODO: change traverse to a template
}

#endif
