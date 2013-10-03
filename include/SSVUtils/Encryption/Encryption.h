// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_ENCRYPTION
#define SSVU_ENCRYPTION

#include <string>
#include "SSVUtils/Encryption/Internal/Base64.h"
#include "SSVUtils/Encryption/Internal/MD5.h"

namespace ssvu
{
	namespace Encryption
	{
		enum class Type{MD5, Base64}; /*!< Enum containing all encryption types. */

		/// @brief Encrypts a string.
		/// @code
		/// using namespace ssvu::Encrpytion;
		///
		/// string s{"test"};
		/// string e{encrypt<Type::Base64>(s)};
		/// strign d{decrypt<Type::Base64>(s)};
		///
		/// assert(s == d);
		/// @endcode
		/// @tparam T Type of encryption to use. (example ssvu::Encryption::Type::MD5)
		/// @param mStr String to encrypt.
		/// @return Returns a string containing the encrypted value.
		template<Type TT> inline std::string encrypt(const std::string& mStr);
		template<> inline std::string encrypt<Type::Base64>(const std::string& mStr)	{ return Internal::Base64Encode(mStr); }
		template<> inline std::string encrypt<Type::MD5>(const std::string& mStr)	{ return Internal::MD5{mStr}.GetHash(); }

		/// @brief Decrypts a string.
		/// @code
		/// using namespace ssvu::Encrpytion;
		/// 
		/// string s{"test"};
		/// string e{encrypt<Type::Base64>(s)};
		/// strign d{decrypt<Type::Base64>(s)};
		/// 
		/// assert(s == d);
		/// @endcode
		/// @tparam T Type of encryption to use. (example ssvu::Encryption::Type::MD5)
		/// @param mStr String to decrypt.
		/// @return Returns a string containing the decrypted value.
		template<Type TT> inline std::string decrypt(const std::string& mStr);
		template<> inline std::string decrypt<Type::Base64>(const std::string& mStr)	{ return Internal::Base64Decode(mStr); }
	}
}

#endif
