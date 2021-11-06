// Copyright (c) 2013-2015 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_CORE_FILESYSTEM_INTERNAL_SCANHELPER
#define SSVU_CORE_FILESYSTEM_INTERNAL_SCANHELPER

#include "SSVUtils/Core/FileSystem/Enums.hpp"
#include "SSVUtils/Core/FileSystem/Path.hpp"

#include <string>
#include <vector>
#include <algorithm>
#include <stdexcept>

#include <dirent.h>
#include <sys/stat.h>

namespace ssvu::FileSystem::Impl
{

template <Mode TM>
[[nodiscard]] inline decltype(auto) getBufName()
{
    // Use a thread-local buffer when the mode is single (non re-entrant),
    // otherwise just create a new string.

    if constexpr(TM == Mode::Recurse)
    {
        return std::string{};
    }
    else
    {
        thread_local std::string bufName;
        return bufName;
    }
}

template <Mode TM>
[[nodiscard]] inline decltype(auto) getBufPath()
{
    // Use a thread-local buffer when the mode is single (non re-entrant),
    // otherwise just create a new path.

    if constexpr(TM == Mode::Recurse)
    {
        return Path{};
    }
    else
    {
        thread_local Path bufPath;
        return bufPath;
    }
}

template <Mode TM, Type TT, Pick TP, Sort TS>
inline void scan(
    std::vector<Path>& mTarget, const Path& mPath, const std::string& mDesired)
{
    if(!mPath.exists<Type::Folder>())
    {
        throw std::runtime_error{"Directory \"" + mPath + "\" not found"};
    }

    DIR* dir{opendir(mPath.getCStr())};

    decltype(auto) bufName = getBufName<TM>();
    decltype(auto) bufPath = getBufPath<TM>();

    for(dirent* entry{readdir(dir)}; entry != nullptr; entry = readdir(dir))
    {
        bufName.clear();
        bufName += entry->d_name;

        bufPath.clear();
        bufPath += mPath;
        bufPath += bufName;

        if(!bufPath.isRootOrParent())
        {
            if(bufPath.template exists<Type::Folder>())
            {
                if(TT == Type::All || TT == Type::Folder)
                {
                    mTarget.emplace_back(bufPath);
                }

                if(TM == Mode::Recurse)
                {
                    Impl::scan<Mode::Recurse, TT, TP, TS>(
                        mTarget, bufPath, mDesired);
                }
            }
            else if(TT == Type::All || TT == Type::File)
            {
                if(TP == Pick::Any)
                {
                    mTarget.emplace_back(bufPath);
                }
                else if(TP == Pick::ByExt)
                {
                    if(endsWith(bufName, mDesired))
                    {
                        mTarget.emplace_back(bufPath);
                    }
                }
                else if(TP == Pick::ByName)
                {
                    if(bufName == mDesired)
                    {
                        mTarget.emplace_back(bufPath);
                    }
                }
            }
        }
    }

    closedir(dir);

    if(TS == Sort::Alphabetic)
    {
        std::sort(std::begin(mTarget), std::end(mTarget));
    }
}

} // namespace ssvu::FileSystem::Impl

#endif
