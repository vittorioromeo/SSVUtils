// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_ENCRYPTION
#define SSVU_ENCRYPTION

#include <string>

namespace ssvu
{
	namespace Encryption
	{
		enum class Type { MD5, BASE64 }; /*!< Enum containing all encryption types. */

		/*!
		 *
		 * @brief Encrypts a string.
		 *
		 * @code
		 * using namespace ssvu::Encrpytion;
		 *
		 * string s{"test"};
		 * string e{encrypt<Type::BASE64>(s)};
		 * strign d{decrypt<Type::BASE64>(s)};
		 *
		 * assert(s == d);
		 * @endcode
		 *
		 * @tparam T Type of encryption to use. (example ssvu::Encryption::Type::MD5)
		 * @param mString String to encrypt.
		 *
		 * @return Returns a string containing the encrypted value.
		 *
		 */
		template<Type T> std::string encrypt(const std::string& mString);

		/*!
		 *
		 * @brief Decrypts a string.
		 *
		 * @code
		 * using namespace ssvu::Encrpytion;
		 *
		 * string s{"test"};
		 * string e{encrypt<Type::BASE64>(s)};
		 * strign d{decrypt<Type::BASE64>(s)};
		 *
		 * assert(s == d);
		 * @endcode
		 *
		 * @tparam T Type of encryption to use. (example ssvu::Encryption::Type::MD5)
		 * @param mString String to decrypt.
		 *
		 * @return Returns a string containing the decrypted value.
		 *
		 */
		template<Type T> std::string decrypt(const std::string& mString);
	}

}

#endif
