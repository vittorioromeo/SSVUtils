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
		 * @brief Fills an std::vector with paths.
		 *
		 * This templated function allows the user to scan the filesystem.
		 *
		 * @tparam TM Mode to use: can be Mode::RECURSIVE or Mode::NON_RECURSIVE.
		 * @tparam TT Type to use: can be Type::ALL (both files and folders), Type::FILES, or Type::FOLDERS
		 * @tparam TP Pick to use: can be Pick::ANY (no restrictions), Pick::BY_EXTENSION (picks only files of a specific extension) or Pick::BY_NAME (picks only files with a specific name)
		 * @param mTarget std::vector to fill.
		 * @param mPath Path to start the search in.
		 * @param mDesired Optional: only set if using Pick::BY_EXTENSION or Pick::BY_NAME.
		 *
		 */
		template<Mode TM = Mode::RECURSIVE, Type TT = Type::ALL, Pick TP = Pick::ANY> void fill(std::vector<std::string>& mTarget, const std::string& mPath, const std::string& mDesired = "")
		{
			FillHelper<TM, TT, TP>::fill(mTarget, mPath, mDesired);
		}

		/*!
		 *
		 * @brief Returns an std::vector filled with paths.
		 *
		 * This templated function allows the user to scan the filesystem.
		 *
		 * @tparam TM Mode to use: can be Mode::RECURSIVE or Mode::NON_RECURSIVE.
		 * @tparam TT Type to use: can be Type::ALL (both files and folders), Type::FILES, or Type::FOLDERS
		 * @tparam TP Pick to use: can be Pick::ANY (no restrictions), Pick::BY_EXTENSION (picks only files of a specific extension) or Pick::BY_NAME (picks only files with a specific name)
		 * @param mPath Path to start the search in.
		 * @param mDesired Optional: only set if using Pick::BY_EXTENSION or Pick::BY_NAME.
		 *
		 * @return Returns a newly created std::vector of strings containing the scanned paths.
		 *
		 */
		template<Mode TM = Mode::RECURSIVE, Type TT = Type::ALL, Pick TP = Pick::ANY> std::vector<std::string> get(const std::string& mPath, const std::string& mDesired = "")
		{
			std::vector<std::string> result;
			fill<TM, TT, TP>(result, mPath, mDesired);
			return result;
		}
	}
}

#endif
