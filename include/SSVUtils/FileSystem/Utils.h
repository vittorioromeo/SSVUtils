// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_FILESYSTEM_UTILS
#define SSVU_FILESYSTEM_UTILS

#include <string>

namespace ssvu
{
	namespace FileSystem
	{
		/*!
		 *
		 * @brief Checks if a path exists on the user's filesystem.
		 *
		 * @param mPath Path to check, must end with '/'.
		 *
		 * @return Returns true if the path exists, false otherwise.
		 *
		 */
		bool exists(const std::string& mPath);

		/*!
		 *
		 * @brief Checks if a filename ends with a specific extension.
		 *
		 * This is a simple string comparison, doesn't actually use the user's filesystem.
		 *
		 * @param mFileName Filename to check, including extension, case unsensitive.
		 * @param mExtension Extension to check, including the dot, case unsensitive.
		 *
		 * @return Returns true if the path exists, false otherwise.
		 *
		 */
		bool hasExtension(const std::string& mFileName, const std::string& mExtension);

		/*!
		 *
		 * @brief Checks if a path on the user's filesystem is a folder.
		 *
		 * @param mPath Path to check, must end with '/'.
		 *
		 * @return Returns true if the path is a folder.
		 *
		 */
		bool isFolder(const std::string& mPath);

		/*!
		 *
		 * @brief Checks if a path ends with '.' or "..".
		 *
		 * @param mPath Path to check.
		 *
		 * @return Returns true if the path ends with '.' or "..".
		 *
		 */
		bool isRootOrParent(const std::string& mPath);

		/*!
		 *
		 * @brief Normalizes a path string.
		 *
		 * Changes '\' to '/', removes useless backslashes at the end.
		 * Modifies the passed string.
		 *
		 * @param mPath Path to normalize.
		 *
		 */
		void normalizePath(std::string& mPath);

		/*!
		 *
		 * @brief Normalizes a path string.
		 *
		 * Changes '\' to '/', removes useless backslashes at the end.
		 * Does not modify the passed string.
		 *
		 * @param mPath Path to normalize.
		 *
		 * @return Returns normalized path string.
		 *
		 */
		std::string getNormalizedPath(std::string mPath);

		/*!
		 *
		 * @brief Returns the parent path of a path. (One upper level)
		 *
		 * Equivalent of remove_leaf in boost::filesystem.
		 *
		 * @param mPath Path to use.
		 *
		 * @return Returns the parent path of mPath.
		 *
		 */
		std::string getParentPath(std::string mPath);

		/*!
		 *
		 * @brief Gets filename from a path.
		 *
		 * Simple string operations, it doesn't depend on the user's filesystem.
		 *
		 * @param mPath Path to use.
		 * @param mPrefix Path before the name.
		 * @param mSuffix Path after the name.
		 *
		 * @return Returns the filename.
		 *
		 */
		std::string getNameFromPath(const std::string& mPath, const std::string& mPrefix, const std::string& mSuffix);

		/*!
		 *
		 * @brief Gets the file's binary contents.
		 *
		 * C-style code to ensure same results on both Linux and Windows.
		 *
		 * @param mPath Path to the file.
		 *
		 * @return Returns a string containing the file's contents.
		 *
		 */
		std::string getFileContents(const std::string& mPath);

		/*!
		 *
		 * @brief Creates a folder on the user's filesystem.
		 *
		 * Works both on Linux and Windows.
		 *
		 * @param mPath Path to non-existing folder, must end with '/'.
		 *
		 */
		void createFolder(std::string mPath);

		/*!
		 *
		 * @brief Removes a file from the user's filesystem.
		 *
		 * @param mPath Path to existing file.
		 *
		 */
		void removeFile(std::string mPath);

		/*!
		 *
		 * @brief Expands the `~` symbol in a path with the user's home path.
		 *
		 * Uses environment variables. Same logic as python's `os.path.expanduserpath`.
		 *
		 * @param mPath Path to expand.
		 *
		 */
		void expandUserPath(std::string& mPath);

		/*!
		 *
		 * @brief Expands the `~` symbol in a path with the user's home path.
		 *
		 * Uses environment variables. Same logic as python's `os.path.expanduserpath`.
		 *
		 * @param mPath Path to expand.
		 *
		 * @return Returns the path with the expanded `~` symbol.
		 *
		 */
		std::string getExpandedUserPath(std::string mPath);
	}
}

#endif
