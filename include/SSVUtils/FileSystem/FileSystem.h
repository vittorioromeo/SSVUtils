// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_FILESYSTEM
#define SSVU_FILESYSTEM

#include <dirent.h>
#include <sys/stat.h>
#include <vector>
#include <string>
#include "SSVUtils/Log/Log.h"
#include "SSVUtils/FileSystem/Internal/FileSystemHelper.h"

namespace ssvu
{
	namespace FileSystem
	{
		/*!
		 *
		 * @brief Fills an existing vector with files (not folders) from a path, non-recursively.
		 *
		 * @param mResult Vector to fill.
		 * @param mPath Path to use.
		 *
		 */
		void fillFiles(std::vector<std::string>& mResult, const std::string& mPath);

		/*!
		 *
		 * @brief Fills an existing vector with files of a specific extension (not folders) from a path, non-recursively.
		 *
		 * @param mResult Vector to fill.
		 * @param mPath Path to use.
		 * @param mExtension Desired file extension.
		 *
		 */
		void fillFilesByExtension(std::vector<std::string>& mResult, const std::string& mPath, const std::string& mExtension);

		/*!
		 *
		 * @brief Fills an existing vector with files of a specific name (not folders) from a path, non-recursively.
		 *
		 * @param mResult Vector to fill.
		 * @param mPath Path to use.
		 * @param mExtension Desired file name.
		 *
		 */
		void fillFilesByName(std::vector<std::string>& mResult, const std::string& mPath, const std::string& mName);

		/*!
		 *
		 * @brief Fills an existing vector with folders (not files) from a path, non-recursively.
		 *
		 * @param mResult Vector to fill.
		 * @param mPath Path to use.
		 *
		 */
		void fillFolders(std::vector<std::string>& mResult, const std::string& mPath);

		/*!
		 *
		 * @brief Fills an existing vector with files and folders from a path, non-recursively.
		 *
		 * @param mResult Vector to fill.
		 * @param mPath Path to use.
		 *
		 */
		void fillAll(std::vector<std::string>& mResult, const std::string& mPath);

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
		 * @param mExtension Desired file extension.
		 *
		 */
		void recursiveFillFilesByExtension(std::vector<std::string>& mResult, const std::string& mPath, const std::string& mExtension);

		/*!
		 *
		 * @brief Fills an existing vector with files of a specific name (not folders) from a path, recursively.
		 *
		 * @param mResult Vector to fill.
		 * @param mPath Path to use.
		 * @param mExtension Desired file name.
		 *
		 */
		void recursiveFillFilesByName(std::vector<std::string>& mResult, const std::string& mPath, const std::string& mName);

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
		 * @param mExtension Desired file extension.
		 *
		 * @return Returns a vector containing the file paths.
		 *
		 */
		std::vector<std::string> getFilesByExtension(const std::string& mPath, const std::string& mExtension);

		/*!
		 *
		 * @brief Returns a vector with files of a specific name (not folders) from a path, non-recursively.
		 *
		 * @param mPath Path to use.
		 * @param mExtension Desired file name.
		 *
		 * @return Returns a vector containing the file paths.
		 *
		 */
		std::vector<std::string> getFilesByName(const std::string& mPath, const std::string& mName);

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
		 * @param mExtension Desired file extension.
		 *
		 * @return Returns a vector containing the file paths.
		 *
		 */
		std::vector<std::string> getRecursiveFilesByExtension(const std::string& mPath, const std::string& mExtension);

		/*!
		 *
		 * @brief Returns a vector with files of a specific name (not folders) from a path, recursively.
		 *
		 * @param mPath Path to use.
		 * @param mName Desired file name.
		 *
		 * @return Returns a vector containing the file paths.
		 *
		 */
		std::vector<std::string> getRecursiveFilesByName(const std::string& mPath, const std::string& mName);

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
	}
}

#endif
