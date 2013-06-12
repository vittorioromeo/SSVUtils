// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_STRING_SPLIT
#define SSVU_STRING_SPLIT

#include <string>
#include <vector>

namespace ssvu
{
	/*!
	 *
	 * @brief Splits a string in smaller strings, filling a target vector<string>.
	 *
	 * @param mTarget Vector to fill with the splitted strings.
	 * @param mString String to split.
	 * @param mSeparator Character to split at. Every occurrence of this character will cause a split.
	 *
	 */
	void split(std::vector<std::string>& mTarget, const std::string& mString, char mSeparator);

	/*!
	 *
	 * @brief Splits a string in smaller strings, filling a target vector<string>.
	 *
	 * @param mTarget Vector to fill with the splitted strings.
	 * @param mString String to split.
	 * @param mSeparator Vector of characters to split at. Every occurrence of any character in the vector will cause a split.
	 *
	 */
	void split(std::vector<std::string>& mTarget, const std::string& mString, const std::vector<char>& mSeparator);

	/*!
	 *
	 * @brief Splits a string in smaller strings, filling a target vector<string>.
	 *
	 * @param mTarget Vector to fill with the splitted strings.
	 * @param mString String to split.
	 * @param mSeparator String to split at. Every occurrence of this string will cause a split.
	 *
	 */
	void split(std::vector<std::string>& mTarget, const std::string& mString, const std::string& mSeparator);

	/*!
	 *
	 * @brief Splits a string in smaller strings, filling a target vector<string>.
	 *
	 * @param mTarget Vector to fill with the splitted strings.
	 * @param mString String to split.
	 * @param mSeparator Vector of strings to split at. Every occurrence of any string in the vector will cause a split.
	 *
	 */
	void split(std::vector<std::string>& mTarget, const std::string& mString, const std::vector<std::string>& mSeparator);

	/*!
	 *
	 * @brief Splits a string in smaller strings, returning a vector<string>.
	 *
	 * @param mString String to split.
	 * @param mSeparator Character to split at. Every occurrence of this character will cause a split.
	 *
	 * @return Returns a std::vector containing all splitted strings.
	 *
	 */
	std::vector<std::string> getSplit(const std::string& mString, char mSeparator);

	/*!
	 *
	 * @brief Splits a string in smaller strings, returning a vector<string>.
	 *
	 * @param mString String to split.
	 * @param mSeparator Vector of characters to split at. Every occurrence of any character in the vector will cause a split.
	 *
	 * @return Returns a std::vector containing all splitted strings.
	 *
	 */
	std::vector<std::string> getSplit(const std::string& mString, const std::vector<char>& mSeparator);

	/*!
	 *
	 * @brief Splits a string in smaller strings, returning a vector<string>.
	 *
	 * @param mString String to split.
	 * @param mSeparator String to split at. Every occurrence of this string will cause a split.
	 *
	 * @return Returns a std::vector containing all splitted strings.
	 *
	 */
	std::vector<std::string> getSplit(const std::string& mString, const std::string& mSeparator);

	/*!
	 *
	 * @brief Splits a string in smaller strings, returning a vector<string>.
	 *
	 * @param mString String to split.
	 * @param mSeparator Vector of strings to split at. Every occurrence of any string in the vector will cause a split.
	 *
	 * @return Returns a std::vector containing all splitted strings.
	 *
	 */
	std::vector<std::string> getSplit(const std::string& mString, const std::vector<std::string>& mSeparator);
}

#endif
