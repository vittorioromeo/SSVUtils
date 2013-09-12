// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_FILESYSTEM_UTILS
#define SSVU_FILESYSTEM_UTILS

#include <cstdlib>
#include <dirent.h>
#include <sys/stat.h>
#include <string>
#include "SSVUtils/String/Utils.h"
#include "SSVUtils/Log/Log.h"
#include "SSVUtils/FileSystem/Path.h"

namespace ssvu
{
	namespace FileSystem
	{
		/// @brief Checks if a path exists on the user's filesystem.
		/// @param mPath Path to check, must end with '/'.
		/// @return Returns true if the path exists, false otherwise.
		inline bool exists(const Path& mPath) { struct stat buf; return stat(mPath.getCStr(), &buf) != -1; }

		/// @brief Gets the file's binary contents.
		/// @details C-style code to ensure same results on both Linux and Windows.
		/// @param mPath Path to the file.
		/// @return Returns a string containing the file's contents.
		inline std::string getFileContents(const Path& mPath)
		{
			std::ifstream ifs{mPath, std::ios_base::binary};
			return {std::istreambuf_iterator<char>(ifs), std::istreambuf_iterator<char>()};
		}

		/// @brief Creates a folder on the user's filesystem.
		/// @details Works both on Linux and Windows.
		/// @param mPath Path to non-existing folder, must end with '/'.
		inline void createFolder(const Path& mPath)
		{
			#ifdef _WIN32
				mkdir(mPath.getCStr());
			#else
				mkdir(mPath.getCStr(), 0755);
			#endif
		}

		/// @brief Removes a file from the user's filesystem.
		/// @param mPath Path to existing file.
		inline void removeFile(const Path& mPath) { if(remove(mPath.getCStr()) != 0) ssvu::lo << ssvu::lt("ssvu::FileSystem::removeFile") << "Error removing file: " << mPath; }

		/// @brief Expands the `~` symbol in a path with the user's home path.
		/// @details Uses environment variables. Same logic as python's `os.path.expanduserpath`.
		/// @param mPath Path to expand.
		inline void expandUserPath(Path& mPath)
		{
			// TODO: WIN32 NEEDS TESTING!

			std::string userHome;

			#ifdef _WIN32
				if(std::getenv("HOME") != NULL) userHome = std::getenv("HOME");
				else if(std::getenv("USERPROFILE") != NULL) userHome = std::getenv("USERPROFILE");
				else if(std::getenv("HOMEPATH") == NULL) return;
				else
				{
					std::string drive{std::getenv("HOMEDRIVE")};
					userHome = drive + std::getenv("HOMEPATH");
				}
			#else
				if(std::getenv("HOME") != NULL) userHome = std::getenv("HOME");
			#endif

			std::string str(mPath);
			replaceAll(str, "~", userHome);
			mPath = str;
		}

		/// @brief Expands the `~` symbol in a path with the user's home path.
		/// @details Uses environment variables. Same logic as python's `os.path.expanduserpath`.
		/// @param mPath Path to expand.
		/// @return Returns the path with the expanded `~` symbol.
		inline Path getExpandedUserPath(Path mPath) { expandUserPath(mPath); return mPath; }
	}
}

#endif
