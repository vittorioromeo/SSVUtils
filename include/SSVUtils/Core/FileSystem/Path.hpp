// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_CORE_FILESYSTEM_PATH
#define SSVU_CORE_FILESYSTEM_PATH

namespace ssvu
{
	namespace FileSystem
	{
		namespace Internal
		{
			template<Type TType> struct ExistsFilter;
			template<> struct ExistsFilter<Type::All>		{ inline static bool get(const CStat& mFileStat) noexcept { return true; } };
			template<> struct ExistsFilter<Type::File>		{ inline static bool get(const CStat& mFileStat) noexcept { return (mFileStat.st_mode & S_IFMT) == S_IFREG; } };
			template<> struct ExistsFilter<Type::Folder>	{ inline static bool get(const CStat& mFileStat) noexcept { return (mFileStat.st_mode & S_IFMT) == S_IFDIR; } };
		}

		class Path
		{
			private:
				mutable std::string path;
				mutable bool mustNormalize{true};

				/// @brief Internal method that normalizes the path string.
				/// @details Removes double slashes. If the path exists as a folder on the filesystem adds a trailing slash.
				inline void normalize() const
				{
					if(!mustNormalize) return;
					mustNormalize = false;
					// TODO: string matcher
					replaceAll(path, R"(\)", "/");
					replaceAll(path, R"(\\)", "/");
					replaceAll(path, "//", "/");
					if(!endsWith(path, "/") && exists<Type::Folder>()) path += "/";
				}

			public:
				inline Path() = default;
				inline Path(const char* mPath) : path{mPath} { }
				inline Path(const std::string& mPath) : path{mPath} { }
				inline Path(std::string&& mPath) : path{std::move(mPath)} { }

				inline const auto& getStr() const		{ normalize(); return path; }
				inline auto getCStr() const noexcept	{ return getStr().c_str(); }

				// TODO: docs, tests, &&
				inline void set(const std::string& mStr) { path = mStr; mustNormalize = true; }
				inline void append(const std::string& mStr) { path += mStr; mustNormalize = true; }

				/// @brief Returns true if the path exists on the user's filesystem and respects the passed filter.
				/// @tparam TType Existance type to check.
				template<Type TType> inline bool exists() const noexcept
				{
					CStat fileStat;
					int err{stat(getCStr(), &fileStat)};
					return err == 0 && Internal::ExistsFilter<TType>::get(fileStat);
				}

				/// @brief Returns true if the path ends with a specified extension. Case-insensitive.
				inline bool hasExtension(const std::string& mExt) const { return endsWith(toLower(getStr()), toLower(mExt)); }

				/// @brief Returns true if the path ends with "./" or "../".
				inline bool isRootOrParent() const { return endsWith(getStr(), "./") || endsWith(getStr(), "../"); }

				/// @brief Returns the parent of the current path, or an empty path if there is no parent.
				/// @details It removes everything after the first found slash starting from the end of the path.
				inline Path getParent() const
				{
					const auto& str(getStr());
					for(auto i(str.size() - 1); i > 0; --i) if(str[i] == '/') return {str.substr(0, i + 1)};
					return {""};
				}

				/// @brief Returns the filename of the current path.
				/// @details It is assumed that the path points to a file.
				inline std::string getFileName() const
				{
					const auto& str(getStr());
					auto nameBegin(str.find_last_of('/') + 1);
					return str.substr(nameBegin, str.size() - nameBegin);
				}

				/// @brief Returns the filename of the current path, without the extension.
				/// @details It is assumed that the path points to a file.
				inline std::string getFileNameNoExtensions() const
				{
					const auto& str(getFileName());
					auto extBegin(str.find_first_of('.', beginsWith(str, '.') ? 1 : 0));
					return str.substr(0, extBegin);
				}

				/// @brief Returns the folder name of the current path.
				/// @details It is assumed that the path points to a folder. Returns an empty string if the path doesn't end with a slash.
				inline std::string getFolderName() const
				{
					const auto& str(getStr());
					if(!endsWith(str, '/')) return "";

					auto i(str.size() - 2);
					while(i > 0 && str[i] != '/') --i;
					return str.substr(i + 1, str.size() - i - 2);
				}

				/// @brief Returns the extension of the current path.
				/// @details It is assumed that the path points to a file.
				inline std::string getExtension() const
				{
					auto str(getFileName()); if(beginsWith(str, '.')) str.erase(std::begin(str));
					auto extBegin(str.find_last_of('.'));
					return extBegin == std::string::npos ? "" : str.substr(extBegin, str.size() - extBegin);
				}

				/// @brief Returns all extensions of the current path. Useful for files with multiple extensions.
				/// @details It is assumed that the path points to a file.
				inline std::string getAllExtensions() const
				{
					const auto& str(getFileName());
					auto extBegin(str.find_first_of('.', beginsWith(str, '.') ? 1 : 0));
					return extBegin == std::string::npos ? "" : str.substr(extBegin, str.size() - extBegin);
				}

				/// @brief Returns a string containing the file's contents, read in binary mode.
				inline std::string getContentsAsString() const
				{
					SSVU_ASSERT(exists<Type::File>());

					std::ifstream ifs{getCStr(), std::ios_base::binary};
					SSVU_ASSERT(ifs);

					ifs.seekg(0, std::ios::end);
					auto size(ifs.tellg());
					ifs.seekg(0, std::ios::beg);

					auto buffer(makeUPtr<char[]>(size));
					ifs.read(&buffer[0], size);
					std::string result{buffer.get(), size};

					return result;
				}

				/// @brief Returns true if the path is empty.
				inline bool isNull() const noexcept { return path == ""; }

				inline bool operator<(const Path& mPath) const { return getStr() < mPath.getStr(); }

				inline operator const std::string&() const { return getStr(); }
		};

		inline std::ostream& operator<<(std::ostream& mStream, const Path& mPath)	{ return mStream << mPath.getStr(); }

		inline Path operator+(const std::string& mLhs, const Path& mRhs)	{ return {mLhs + mRhs.getStr()}; }
		inline Path operator+(const Path& mLhs, const std::string& mRhs)	{ return {mLhs.getStr() + mRhs}; }
		inline Path operator+(const Path& mLhs, const Path& mRhs)			{ return {mLhs.getStr() + mRhs.getStr()}; }
		inline Path operator+(const char* mLhs, const Path& mRhs)			{ return {std::string(mLhs) + mRhs.getStr()}; }
		inline Path operator+(const Path& mLhs, const char* mRhs)			{ return {mLhs.getStr() + std::string(mRhs)}; }

		inline Path& operator+=(Path& mLhs, const std::string& mRhs)		{ mLhs.append(mRhs); return mLhs; }
		inline Path& operator+=(Path& mLhs, const Path& mRhs)				{ mLhs.append(mRhs.getStr()); return mLhs; }
		inline Path& operator+=(Path& mLhs, const char* mRhs)				{ mLhs.append(std::string{mRhs}); return mLhs; }

		inline bool operator==(const std::string& mLhs, const Path& mRhs)	{ return mLhs == mRhs.getStr(); }
		inline bool operator==(const Path& mLhs, const std::string& mRhs)	{ return mLhs.getStr() == mRhs; }
		inline bool operator==(const Path& mLhs, const Path& mRhs)			{ return mLhs.getStr() == mRhs.getStr(); }
		inline bool operator==(const char* mLhs, const Path& mRhs)			{ return std::string(mLhs) == mRhs.getStr(); }
		inline bool operator==(const Path& mLhs, const char* mRhs)			{ return mLhs.getStr() == std::string(mRhs); }
	}
}

#endif
