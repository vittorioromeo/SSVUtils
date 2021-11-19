// Copyright (c) 2013-2015 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_CORE_FILESYSTEM_PATH
#define SSVU_CORE_FILESYSTEM_PATH

#include "SSVUtils/Core/FileSystem/Enums.hpp"
#include "SSVUtils/Core/FileSystem/CStat.hpp"

#include "SSVUtils/Core/Common/Aliases.hpp"
#include "SSVUtils/Core/Utils/Macros.hpp"
#include "SSVUtils/Core/String/Utils.hpp"

#include <string>
#include <utility>
#include <fstream>
#include <cassert>
#include <sstream>

#include <dirent.h>
#include <sys/stat.h>

namespace ssvu
{
namespace FileSystem
{
namespace Impl
{
template <Type TType>
struct ExistsFilter;
template <>
struct ExistsFilter<Type::All>
{
    inline static bool get(const CStat&) noexcept
    {
        return true;
    }
};
template <>
struct ExistsFilter<Type::File>
{
    inline static bool get(const CStat& mStat) noexcept
    {
        return (mStat.st_mode & S_IFMT) == S_IFREG;
    }
};
template <>
struct ExistsFilter<Type::Folder>
{
    inline static bool get(const CStat& mStat) noexcept
    {
        return (mStat.st_mode & S_IFMT) == S_IFDIR;
    }
};
} // namespace Impl

class Path
{
private:
    mutable std::string path;
    mutable bool mustNormalize{true};

    /// @brief Internal method that normalizes the path string.
    /// @details Removes double slashes. If the path exists as a folder
    /// on the filesystem adds a trailing slash.
    inline void normalize() const noexcept
    {
        if(!mustNormalize) return;
        mustNormalize = false;

        // Replace every '\' with '/'
        for(auto& c : path)
            if(c == '\\') c = '/';

        // Replace consecutive '/' with a single '/'
        for(auto i(0u); i < path.size(); ++i)
        {
            if(path[i] != '/') continue;

            auto j(i);
            while(j < path.size() && path[j] == '/') ++j;

            path.erase(i, j - i - 1);
        }

        if(!endsWith(path, "/") && exists<Type::Folder>()) path += "/";
    }

public:
    inline Path() = default;
    inline Path(const Path& mPath) : path{mPath.path}
    {
    }
    inline Path(Path&& mPath) noexcept : path{std::move(mPath.path)}
    {
    }
    inline Path(const std::string& mPath) : path{mPath}
    {
    }
    inline Path(std::string&& mPath) noexcept : path{std::move(mPath)}
    {
    }
    inline Path(const char* mPath) : path{mPath}
    {
    }

    void clear()
    {
        path.clear();
        mustNormalize = true;
    }

    inline auto& operator=(const Path& mPath)
    {
        path = mPath.path;
        mustNormalize = true;
        return *this;
    }
    inline auto& operator=(Path&& mPath) noexcept
    {
        path = std::move(mPath.path);
        mustNormalize = true;
        return *this;
    }

    template <typename T, SSVU_ENABLEIF_RA_IS_NOT(T, Path)>
    inline auto& operator=(T&& mPath) noexcept(noexcept(path = FWD(mPath)))
    {
        path = FWD(mPath);
        mustNormalize = true;
        return *this;
    }

    template <typename T, SSVU_ENABLEIF_RA_IS(T, Path)>
    inline auto& operator+=(T&& mPath) noexcept(
        noexcept(path += moveIfRValue<decltype(mPath)>(mPath.path)))
    {
        path += moveIfRValue<decltype(mPath)>(mPath.path);
        mustNormalize = true;
        return *this;
    }
    template <typename T, SSVU_ENABLEIF_RA_IS_NOT(T, Path)>
    inline auto& operator+=(T&& mPath) noexcept(noexcept(path += FWD(mPath)))
    {
        path += FWD(mPath);
        mustNormalize = true;
        return *this;
    }

    // Internal path string getters
    inline const std::string& getStr() & noexcept
    {
        normalize();
        return path;
    }
    inline const std::string& getStr() const& noexcept
    {
        normalize();
        return path;
    }
    inline std::string&& getStr() && noexcept
    {
        normalize();
        return std::move(path);
    }

    // Implicit conversion to `std::string`
    inline operator const std::string&() & noexcept
    {
        return getStr();
    }
    inline operator const std::string&() const& noexcept
    {
        return getStr();
    }
    inline operator std::string() && noexcept
    {
        return std::move(std::move(*this).getStr());
    }

    inline auto getCStr() const noexcept
    {
        return getStr().c_str();
    }

    /// @brief Returns true if the path exists on the user's filesystem
    /// and respects the passed filter.
    /// @tparam TType Existence type to check.
    template <Type TType>
    inline bool exists() const noexcept
    {
        CStat fileStat;
        int err{stat(getCStr(), &fileStat)};
        return err == 0 && Impl::ExistsFilter<TType>::get(fileStat);
    }

    inline bool isFolder() const noexcept
    {
        return exists<Type::Folder>();
    }

    inline bool isFile() const noexcept
    {
        return exists<Type::File>();
    }

    /// @brief Returns true if the path ends with a specified extension.
    /// Case-insensitive.
    inline bool hasExtension(const std::string& mExt) const
    {
        return endsWith(toLower(getStr()), toLower(mExt));
    }

    /// @brief Returns true if the path ends with "./" or "../".
    inline bool isRootOrParent() const
    {
        return endsWith(getStr(), "./") || endsWith(getStr(), "../");
    }

    /// @brief Returns the parent of the current path, or an empty path
    /// if there is no parent.
    /// @details It removes everything after the first found slash
    /// starting from the end of the path.
    inline Path getParent() const
    {
        const auto& str(getStr());
        for(auto i(str.size() - 1); i > 0; --i)
            if(str[i] == '/') return {str.substr(0, i + 1)};
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

    /// @brief Returns the filename of the current path, without the
    /// extension.
    /// @details It is assumed that the path points to a file.
    inline std::string getFileNameNoExtensions() const
    {
        const auto& str(getStr());
        auto nameBegin(str.find_last_of('/') + 1);
        auto extBegin(str.find_last_of('.'));

        if(extBegin == nameBegin)
        {
            return str.substr(nameBegin);
        }

        return str.substr(nameBegin, extBegin - nameBegin);
    }

    /// @brief Returns the folder name of the current path.
    /// @details It is assumed that the path points to a folder. Returns
    /// an empty string if the path doesn't end with a slash.
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
        auto str(getFileName());
        if(beginsWith(str, '.')) str.erase(std::begin(str));
        auto extBegin(str.find_last_of('.'));
        return extBegin == std::string::npos
                   ? ""
                   : str.substr(extBegin, str.size() - extBegin);
    }

    /// @brief Returns all extensions of the current path. Useful for
    /// files with multiple extensions.
    /// @details It is assumed that the path points to a file.
    inline std::string getAllExtensions() const
    {
        const auto& str(getFileName());
        auto extBegin(str.find_first_of('.', beginsWith(str, '.') ? 1 : 0));
        return extBegin == std::string::npos
                   ? ""
                   : str.substr(extBegin, str.size() - extBegin);
    }

    /// @brief Returns a string containing the file's contents, read in
    /// binary mode, using a user-provided buffer.
    inline std::string& getContentsAsStr(std::string& mBuffer) const
    {
        assert(exists<Type::File>());

        std::ifstream ifs{getCStr(), std::ios_base::binary};
        assert(static_cast<bool>(ifs));

        ifs.seekg(0, std::ios::end);
        mBuffer.resize(ifs.tellg());
        ifs.seekg(0);
        ifs.read(mBuffer.data(), mBuffer.size());

        return mBuffer;
    }

    /// @brief Returns a string containing the file's contents, read in
    /// binary mode.
    inline std::string getContentsAsStr() const
    {
        std::string buffer;
        return getContentsAsStr(buffer);
    }

    /// @brief Returns true if the path is empty.
    inline bool isNull() const noexcept
    {
        return path.empty();
    }

    inline bool operator<(const Path& mPath) const noexcept
    {
        return getStr() < mPath.getStr();
    }
};

inline std::ostream& operator<<(std::ostream& mStream, const Path& mPath)
{
    return mStream << mPath.getStr();
}

#define ENABLEIF_ANY_PATH()                                                    \
    std::enable_if_t<                                                          \
        std::is_same_v<std::remove_cv_t<std::remove_reference_t<T1>>, Path> || \
        std::is_same_v<std::remove_cv_t<std::remove_reference_t<T2>>,          \
            Path>>* = nullptr

// Operator+
template <typename T1, typename T2, ENABLEIF_ANY_PATH()>
inline Path operator+(T1&& mLhs, T2&& mRhs)
{
    Path result{FWD(mLhs)};
    result += FWD(mRhs);
    return result;
}

// Equality/inequality check
template <typename T1, typename T2, ENABLEIF_ANY_PATH()>
inline bool operator==(const T1& mLhs, const T2& mRhs)
{
    return mLhs == mRhs;
}
template <typename T1, typename T2, ENABLEIF_ANY_PATH()>
inline bool operator!=(const T1& mLhs, const T2& mRhs)
{
    return mLhs != mRhs;
}

#undef ENABLEIF_ANY_PATH
} // namespace FileSystem
} // namespace ssvu

#endif
