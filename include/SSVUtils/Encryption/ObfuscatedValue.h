// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_ENCRYPTION_OBFUSCATEDVALUE
#define SSVU_ENCRYPTION_OBFUSCATEDVALUE

#include <string>
#include <sstream>
#include <type_traits>
#include "SSVUtils/Encryption/Encryption.h"

namespace ssvu
{
	template<typename T, Encryption::Type TCrypto = Encryption::Type::Base64, typename TEnable = void> class ObfuscatedValue;

	/*!
	 *
	 * @brief Base64 "obfuscated" value
	 *
	 * Quick (but not really effective) way to protect a value against memory scanners (such as Cheat Engine).
	 * Obviously introduces a runtime cost to get/set the internal value.
	 *
	 * @tparam T Type of the underlying arithmetic value.
	 *
	 */
	template<typename T, Encryption::Type TCrypto> class ObfuscatedValue<T, TCrypto, typename std::enable_if<std::is_arithmetic<T>::value>::type>
	{
		private:
			T dummy; /*!< Dummy value used to "fool" memory scanners. */
			std::string encodedValue; /*!< The "real" value, under the form of a Base64 string. */

			/*!
			 *
			 * @brief Converts the encodedValue to the arithmetic type.
			 *
			 * Used internally.
			 *
			 * @param mString String to convert to arithmetic type.
			 *
			 */
			T fromString(const std::string& mString) const { std::istringstream stream(mString); T t; stream >> t; return t; }

		public:
			/*!
			 *
			 * @brief Constructs an ObfuscatedValue from an arithmetic value
			 *
			 * @param mValue The arithmetic value that will be stored internally
			 *
			 */
			ObfuscatedValue(T mValue) { set(mValue); }

			/*!
			 *
			 * @brief Sets the internal Base64 string
			 *
			 * @param mValue Value to use
			 *
			 */
			void set(T mValue) { dummy = mValue; encodedValue = Encryption::encrypt<TCrypto>(toStr(mValue)); }

			/*!
			 *
			 * @brief Converts the internal Base64 string and returns the unobfuscated value
			 *
			 */
			T get() const { return fromString(Encryption::decrypt<TCrypto>(encodedValue)); }

			/*!
			 *
			 * @brief Implicit conversion to the obfuscated type
			 *
			 */
			operator T() const { return get(); }

			/*!
			 *
			 * @brief Adds a value to the internal one
			 *
			 * @param mValue Value to add
			 *
			 */
			T operator +=(const T& mValue) { set(get() + mValue); return get(); }

			/*!
			 *
			 * @brief Subtracts a value from the internal one
			 *
			 * @param mValue Value to subtract
			 *
			 */
			T operator -=(const T& mValue) { set(get() - mValue); return get(); }

			/*!
			 *
			 * @brief Multiplies the internal value by another value
			 *
			 * @param mValue Value to multiply with
			 *
			 */
			T operator *=(const T& mValue) { set(get() * mValue); return get(); }

			/*!
			 *
			 * @brief Divides the internal value by another value
			 *
			 * @param mValue Value to divide with
			 *
			 */
			T operator /=(const T& mValue) { set(get() / mValue); return get(); }

			/*!
			 *
			 * @brief Shortcut operator for set
			 *
			 * @param mValue Value to use
			 *
			 */
			void operator =(const T& mValue) { set(mValue); }
	};
}

#endif
