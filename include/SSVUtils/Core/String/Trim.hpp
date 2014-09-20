// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_CORE_STRING_TRIM
#define SSVU_CORE_STRING_TRIM

namespace ssvu
{
	/// @brief Trims all characters of a certain type from the string's left.
	/// @param mStr String to trim. (will be modified)
	/// @param mChar Character to trim.
	inline void trimStrL(std::string& mStr, char mChar = ' ') { trimL<std::string>(mStr, [mChar](char mC){ return mC == mChar; }); }

	/// @brief Trims all characters of a certain type from the string's right.
	/// @param mStr String to trim. (will be modified)
	/// @param mChar Character to trim.
	inline void trimStrR(std::string& mStr, char mChar = ' ') { trimR<std::string>(mStr, [mChar](char mC){ return mC == mChar; }); }

	/// @brief Trims all characters of a certain type both from the string's left and right.
	/// @param mStr String to trim. (will be modified)
	/// @param mChar Character to trim.
	inline void trimStrLR(std::string& mStr, char mChar = ' ') { trimLR<std::string>(mStr, [mChar](char mC){ return mC == mChar; }); }

	/// @brief Gets a copy of a string with characters of a certain type trimmed from the left.
	/// @param mStr String to trim. (will not be modified)
	/// @param mChar Character to trim.
	/// @return Returns a copy of the string, trimmed.
	inline auto getTrimStrL(std::string mStr, char mChar = ' ') { trimStrL(mStr, mChar); return mStr; }

	/// @brief Gets a copy of a string with characters of a certain type trimmed from the right.
	/// @param mStr String to trim. (will not be modified)
	/// @param mChar Character to trim.
	/// @return Returns a copy of the string, trimmed.
	inline auto getTrimStrR(std::string mStr, char mChar = ' ') { trimStrR(mStr, mChar); return mStr; }

	/// @brief Gets a copy of a string with characters of a certain type trimmed both from the left and right.
	/// @param mStr String to trim. (will not be modified)
	/// @param mChar Character to trim.
	/// @return Returns a copy of the string, trimmed.
	inline auto getTrimStrLR(std::string mStr, char mChar = ' ') { trimStrLR(mStr, mChar); return mStr; }
}

#endif
