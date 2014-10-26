// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_CORE_FILESYSTEM_UTILS
#define SSVU_CORE_FILESYSTEM_UTILS

namespace ssvu
{
	namespace FileSystem
	{
		/// @brief Creates a folder on the user's filesystem.
		/// @details Works both on Linux and Windows.
		/// @param mPath Path to non-existing folder, must end with '/'.
		inline void createFolder(const Path& mPath)
		{
			#if defined(SSVU_OS_WINDOWS)
				mkdir(mPath.getCStr());
			#else
				mkdir(mPath.getCStr(), 0755);
			#endif
		}

		/// @brief Removes a file from the user's filesystem.
		/// @param mPath Path to existing file.
		inline bool removeFile(const Path& mPath) { return std::remove(mPath.getCStr()); }

		/// @brief Expands the `~` symbol in a path with the user's home path.
		/// @details Uses environment variables. Same logic as python's `os.path.expanduserpath`.
		/// @param mPath Path to expand.
		inline void expandUserPath(Path& mPath)
		{
			// TODO: WIN32 NEEDS TESTING!

			std::string userHome;

			#if defined(SSVU_OS_WINDOWS)
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

			mPath = getReplacedAll(mPath, "~", userHome);
		}

		/// @brief Expands the `~` symbol in a path with the user's home path.
		/// @details Uses environment variables. Same logic as python's `os.path.expanduserpath`.
		/// @param mPath Path to expand.
		/// @return Returns the path with the expanded `~` symbol.
		inline Path getExpandedUserPath(Path mPath) { expandUserPath(mPath); return mPath; }
	}
}

#endif
