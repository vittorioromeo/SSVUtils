// Copyright (c) 2013-2015 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_CORE_LOG_INL
#define SSVU_CORE_LOG_INL

#include "SSVUtils/Core/FileSystem/Path.hpp"

#include <fstream>

namespace ssvu
{
#ifndef SSVU_LOG_DISABLE
/// @brief Saves all the log entries to a file.
/// @param mPath File path (file will be created if it doesn't exist).
inline void saveLogToFile(const ssvu::FileSystem::Path& mPath)
{
    std::ofstream o;
    o.open(mPath);
    o << getLogStream().str();
    o.flush();
    o.close();
}
#else
inline void saveLogToFile(const ssvu::FileSystem::Path&)
{
}
#endif
} // namespace ssvu

#endif
