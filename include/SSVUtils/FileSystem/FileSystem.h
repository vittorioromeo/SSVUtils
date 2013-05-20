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
		 *
		 * @param mPath Path to normalize.
		 *
		 * @return Returns normalized path string.
		 *
		 */
		std::string getNormalizedPath(const std::string& mPath);

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
		 * @brief Internal method used with in all other functions.
		 *
		 */
		void traverse(const std::string& mPath, std::function<void(std::string, std::string)> mFunction);

		/*!
		 *
		 * @brief Fills an existing vector with files (not folders) from a path, recursively.
		 *
		 * @param mResult Vector to fill.
		 * @param mPath Path to use.
		 *
		 */
		void recursiveFillFiles(std::vector<std::string>& mResult, const std::string& mPath);

		/*!
		 *
		 * @brief Fills an existing vector with files of a specific extension (not folders) from a path, recursively.
		 *
		 * @param mResult Vector to fill.
		 * @param mPath Path to use.
		 * @param mExtension Extension to use.
		 *
		 */
		void recursiveFillFilesByExtension(std::vector<std::string>& mResult, const std::string& mPath, const std::string& mExtension);

		/*!
		 *
		 * @brief Fills an existing vector with folders (not files) from a path, recursively.
		 *
		 * @param mResult Vector to fill.
		 * @param mPath Path to use.
		 *
		 */
		void recursiveFillFolders(std::vector<std::string>& mResult, const std::string& mPath);

		/*!
		 *
		 * @brief Fills an existing vector with files and folders from a path, recursively.
		 *
		 * @param mResult Vector to fill.
		 * @param mPath Path to use.
		 *
		 */
		void recursiveFillAll(std::vector<std::string>& mResult, const std::string& mPath);

		/*!
		 *
		 * @brief Returns a vector with files (not folders) from a path, non-recursively.
		 *
		 * @param mPath Path to use.
		 *
		 * @return Returns a vector containing the file paths.
		 *
		 */
		std::vector<std::string> getFiles(const std::string& mPath);

		/*!
		 *
		 * @brief Returns a vector with files of a specific extension (not folders) from a path, non-recursively.
		 *
		 * @param mPath Path to use.
		 * @param mExtension Extension to use.
		 *
		 * @return Returns a vector containing the file paths.
		 *
		 */
		std::vector<std::string> getFilesByExtension(const std::string& mPath, const std::string& mExtension);

		/*!
		 *
		 * @brief Returns a vector with folders (not files) from a path, non-recursively.
		 *
		 * @param mPath Path to use.
		 *
		 * @return Returns a vector containing the folder paths.
		 *
		 */
		std::vector<std::string> getFolders(const std::string& mPath);

		/*!
		 *
		 * @brief Returns a vector with files and folders, non-recursively.
		 *
		 * @param mPath Path to use.
		 *
		 * @return Returns a vector containing the file and folder paths.
		 *
		 */
		std::vector<std::string> getAll(const std::string& mPath);

		/*!
		 *
		 * @brief Returns a vector with files (not folders) from a path, recursively.
		 *
		 * @param mPath Path to use.
		 *
		 * @return Returns a vector containing the file paths.
		 *
		 */
		std::vector<std::string> getRecursiveFiles(const std::string& mPath);

		/*!
		 *
		 * @brief Returns a vector with files of a specific extension (not folders) from a path, recursively.
		 *
		 * @param mPath Path to use.
		 * @param mExtension Extension to use.
		 *
		 * @return Returns a vector containing the file paths.
		 *
		 */
		std::vector<std::string> getRecursiveFilesByExtension(const std::string& mPath, const std::string& mExtension);

		/*!
		 *
		 * @brief Returns a vector with folders (not files) from a path, recursively.
		 *
		 * @param mPath Path to use.
		 *
		 * @return Returns a vector containing the folder paths.
		 *
		 */
		std::vector<std::string> getRecursiveFolders(const std::string& mPath);

		/*!
		 *
		 * @brief Returns a vector with files and folders, recursively.
		 *
		 * @param mPath Path to use.
		 *
		 * @return Returns a vector containing the file and folder paths.
		 *
		 */
		std::vector<std::string> getRecursiveAll(const std::string& mPath);

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
		void createFolder(const std::string& mPath);
	}
}

#endif
