// Copyright (c) 2013-2015 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_IMPL_ENCRYPTION
#define SSVU_IMPL_ENCRYPTION

#include "SSVUtils/Core/Core.hpp"

namespace ssvu
{
namespace Encoding
{
enum class Type
{
    MD5,
    Base64
}; /*!< Enum containing all encryption types. */

/// @brief Encrypts a string.
/// @code
/// using namespace ssvu::Encoding;
///
/// string s{"test"};
/// string e{encode<Type::Base64>(s)};
/// strign d{decode<Type::Base64>(s)};
///
/// SSVU_ASSERT(s == d);
/// @endcode
/// @tparam TT Type of encryption to use. (example
/// ssvu::Encoding::Type::MD5)
/// @param mStr String to encrypt.
/// @return Returns a string containing the encrypted value.
template <Type TT>
std::string encode(const std::string& mStr);

/// @brief Decrypts a string.
/// @code
/// using namespace ssvu::Encoding;
///
/// string s{"test"};
/// string e{encode<Type::Base64>(s)};
/// strign d{decode<Type::Base64>(s)};
///
/// SSVU_ASSERT(s == d);
/// @endcode
/// @tparam TT Type of encryption to use. (example
/// ssvu::Encoding::Type::MD5)
/// @param mStr String to decrypt.
/// @return Returns a string containing the decrypted value.
template <Type TT>
std::string decode(const std::string& mStr);
} // namespace Encoding
} // namespace ssvu

#endif
