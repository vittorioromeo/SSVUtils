// Copyright (c) 2013-2015 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_JSON_IO_READEXCEPTION
#define SSVU_JSON_IO_READEXCEPTION

#include "SSVUtils/Core/Core.hpp"

namespace ssvu
{
namespace Json
{
class ReadException : public std::runtime_error
{
private:
    std::string title, src;

public:
    inline ReadException(
        std::string mTitle, std::string mWhat, std::string mSrc)
        : std::runtime_error{std::move(mWhat)}, title{std::move(mTitle)},
          src{std::move(mSrc)}
    {
    }

    inline const auto& getTitle() const noexcept
    {
        return title;
    }
    inline const auto& getSrc() const noexcept
    {
        return src;
    }
};
} // namespace Json
} // namespace ssvu

#endif
