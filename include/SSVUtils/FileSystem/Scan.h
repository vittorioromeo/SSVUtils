// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_FILESYSTEM_SCAN
#define SSVU_FILESYSTEM_SCAN

#include <dirent.h>
#include <sys/stat.h>
#include <vector>
#include <string>
#include "SSVUtils/Log/Log.h"
#include "SSVUtils/FileSystem/Enums.h"
#include "SSVUtils/FileSystem/Internal/ScanHelper.h"

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
		 * @code
		 * using namespace std;
		 * using namespace ssvu::FileSystem;
		 * vector<string> usrFiles, homeFolders;
		 *
		 * fill<Mode::Recurse, Type::File>(usrFiles, "/usr/");
		 * fill<Mode::Recurse, Type::Folder>(homeFolders, "/home/");
		 * @endcode
		 *
		 * @tparam TM Mode to use: can be Mode::Recurse or Mode::Single.
		 * @tparam TT Type to use: can be Type::All (both files and folders), Type::File, or Type::Folder
		 * @tparam TP Pick to use: can be Pick::Any (no restrictions), Pick::ByExt (picks only files of a specific extension) or Pick::ByName (picks only files with a specific name)
		 * @tparam TS Sort to use: can be Sort::Alphabetic or Sort::Unsorted
		 * @param mTarget std::vector to fill.
		 * @param mPath Path to start the search in.
		 * @param mDesired Optional: only set if using Pick::ByExt or Pick::ByName.
		 *
		 */
		template<Mode TM = Mode::Recurse, Type TT = Type::All, Pick TP = Pick::Any, Sort TS = Sort::Alphabetic> void scan(std::vector<std::string>& mTarget, const std::string& mPath, const std::string& mDesired = "")
		{
			Internal::scan<TM, TT, TP, TS>(mTarget, mPath, mDesired);
		}

		/*!
		 *
		 * @brief Returns an std::vector filled with paths.
		 *
		 * This templated function allows the user to scan the filesystem.
		 *
		 * @code
		 * using namespace std;
		 * using namespace ssvu::FileSystem;
		 * vector<string> usrFiles{get<Mode::Recurse, Type::File>(usrFiles, "/usr/")};
		 * vector<string> homeFolders{get<Mode::Recurse, Type::Folder>(homeFolders, "/home/")};
		 * @endcode
		 *
		 * @tparam TM Mode to use: can be Mode::Recurse or Mode::Single.
		 * @tparam TT Type to use: can be Type::All (both files and folders), Type::File, or Type::Folder
		 * @tparam TP Pick to use: can be Pick::Any (no restrictions), Pick::ByExt (picks only files of a specific extension) or Pick::ByName (picks only files with a specific name)
		 * @tparam TS Sort to use: can be Sort::Alphabetic or Sort::Unsorted
		 * @param mPath Path to start the search in.
		 * @param mDesired Optional: only set if using Pick::ByExt or Pick::ByName.
		 *
		 * @return Returns a newly created std::vector of strings containing the scanned paths.
		 *
		 */
		template<Mode TM = Mode::Recurse, Type TT = Type::All, Pick TP = Pick::Any, Sort TS = Sort::Alphabetic> std::vector<std::string> getScan(const std::string& mPath, const std::string& mDesired = "")
		{
			std::vector<std::string> result;
			scan<TM, TT, TP, TS>(result, mPath, mDesired);
			return result;
		}
	}
}

#endif
