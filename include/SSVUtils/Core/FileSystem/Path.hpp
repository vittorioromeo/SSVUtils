// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_CORE_FILESYSTEM_PATH
#define SSVU_CORE_FILESYSTEM_PATH

#include <cassert>
#include <cstdlib>
#include <dirent.h>
#include <sys/stat.h>
#include <iostream>
#include <string>
#include "SSVUtils/Core/FileSystem/Enums.hpp"
#include "SSVUtils/Core/FileSystem/Utils.hpp"
#include "SSVUtils/Core/String/Utils.hpp"

namespace ssvu
{
	namespace FileSystem
	{
		class Path
		{
			private:
				mutable std::string path;
				mutable bool mustNormalize{true};

				inline void normalize() const
				{
					if(!mustNormalize) return;
					mustNormalize = false;
					replaceAll(path, R"(\)", "/");
					replaceAll(path, R"(\\)", "/");
					replaceAll(path, "//", "/");
					if(!endsWith(path, "/") && existsAsFolder()) path += "/";
				}

			public:
				inline Path() = default;
				inline Path(const char* mPath) : path{mPath} { }
				inline Path(std::string mPath) : path{std::move(mPath)} { }

				inline const std::string& getStr() const	{ normalize(); return path; }
				inline const char* getCStr() const noexcept	{ return getStr().c_str(); }

				inline bool existsAsFolder() const
				{
					struct stat fileStat;
					int err{stat(getCStr(), &fileStat)};
					if(err != 0) return false;
					return (fileStat.st_mode & S_IFMT) == S_IFDIR;
				}

				inline bool hasExtension(const std::string& mExt) const	{ return endsWith(toLower(getStr()), toLower(mExt)); }
				inline bool exists() const								{ struct stat buf; return stat(getCStr(), &buf) != -1; }
				inline bool isRootOrParent() const						{ return endsWith(getStr(), "./") || endsWith(getStr(), "../"); }
				inline Path getParent() const
				{
					const auto& str(getStr());
					for(auto i(str.size() - 1); i > 0; --i) if(str[i] == '/') return {str.substr(0, i + 1)};
					return {""};
				}
				inline std::string getFileName() const
				{
					const auto& str(getStr());
					auto nameBegin(str.find_last_of('/') + 1);
					return str.substr(nameBegin, str.size() - nameBegin);
				}
				inline std::string getFileNameNoExtensions() const
				{
					const auto& str(getFileName());
					auto extBegin(str.find_first_of('.', beginsWith(str, '.') ? 1 : 0));
					return str.substr(0, extBegin);
				}
				inline std::string getFolderName() const
				{
					auto str(getStr());
					if(!endsWith(str, '/')) return "";

					str.erase(std::end(str) - 1);
					auto nameBegin(str.find_last_of('/') + 1);
					return str.substr(nameBegin, str.size() - nameBegin);
				}
				inline std::string getExtension() const
				{
					auto str(getFileName()); if(beginsWith(str, '.')) str.erase(std::begin(str));
					auto extBegin(str.find_last_of('.'));
					return extBegin == std::string::npos ? "" : str.substr(extBegin, str.size() - extBegin);
				}
				inline std::string getAllExtensions() const
				{
					const auto& str(getFileName());
					auto extBegin(str.find_first_of('.', beginsWith(str, '.') ? 1 : 0));
					return extBegin == std::string::npos ? "" : str.substr(extBegin, str.size() - extBegin);
				}

				inline bool isNull() const noexcept { return path == ""; }
				inline bool operator<(const Path& mPath) const { return getStr() < mPath.getStr(); }

				inline operator const std::string&() const { return getStr(); }
		};

		inline std::ostream& operator<<(std::ostream& mStream, const Path& mPath)	{ return mStream << mPath.getStr(); }

		inline Path operator+(const std::string& mLhs, const Path& mRhs)			{ return {mLhs + mRhs.getStr()}; }
		inline Path operator+(const Path& mLhs, const std::string& mRhs)			{ return {mLhs.getStr() + mRhs}; }
		inline Path operator+(const Path& mLhs, const Path& mRhs)					{ return {mLhs.getStr() + mRhs.getStr()}; }
		inline Path operator+(const char* mLhs, const Path& mRhs)					{ return {std::string(mLhs) + mRhs.getStr()}; }
		inline Path operator+(const Path& mLhs, const char* mRhs)					{ return {mLhs.getStr() + std::string(mRhs)}; }

		inline bool operator==(const std::string& mLhs, const Path& mRhs)			{ return mLhs == mRhs.getStr(); }
		inline bool operator==(const Path& mLhs, const std::string& mRhs)			{ return mLhs.getStr() == mRhs; }
		inline bool operator==(const Path& mLhs, const Path& mRhs)					{ return mLhs.getStr() == mRhs.getStr(); }
		inline bool operator==(const char* mLhs, const Path& mRhs)					{ return std::string(mLhs) == mRhs.getStr(); }
		inline bool operator==(const Path& mLhs, const char* mRhs)					{ return mLhs.getStr() == std::string(mRhs); }
	}
}

#endif

// TODO: docs
