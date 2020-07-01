// Copyright (c) 2013-2015 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#include "SSVUtils/Internal/API.hpp"
#include "SSVUtils/Core/Core.hpp"
#include "SSVUtils/Encoding/Inc/Encoding.hpp"
#include "SSVUtils/Encoding/Inc/Internal/Base64.hpp"
#include "SSVUtils/Encoding/Inc/Internal/MD5.hpp"

namespace ssvu
{
namespace Encoding
{
template <>
SSVU_INLINE std::string encode<Type::Base64>(const std::string& mStr)
{
    return Impl::Base64Encode(mStr);
}
template <>
SSVU_INLINE std::string encode<Type::MD5>(const std::string& mStr)
{
    return Impl::MD5{mStr}.GetHash();
}

template <>
SSVU_INLINE std::string decode<Type::Base64>(const std::string& mStr)
{
    return Impl::Base64Decode(mStr);
}
} // namespace Encoding
} // namespace ssvu
