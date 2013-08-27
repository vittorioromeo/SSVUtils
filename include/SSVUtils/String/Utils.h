// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_STRING_UTILS
#define SSVU_STRING_UTILS

#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include "SSVUtils/Global/Typedefs.h"

namespace ssvu
{
	/*!
	 *
	 * @brief Converts a value to a std::string
	 *
	 * Uses std::ostringstream internally.
	 *
	 * @tparam T Type of value.
	 * @param mValue Const reference to the value. (original value won't be changed)
	 *
	 * @return Returns a std::string representing the converted value.
	 *
	 */
	template<typename T> inline std::string toStr(const T& mValue) { std::ostringstream oss; oss << mValue; return oss.str(); }

	/*!
	 *
	 * @brief Replace the first occurrence of a string in a string with another string.
	 *
	 * @param mString String to work with.
	 * @param mFrom String to replace.
	 * @param mTo Replacement string.
	 *
	 */
	inline void replace(std::string& mString, const std::string& mFrom, const std::string& mTo)
	{
		size_t startPos{mString.find(mFrom)};
		if(startPos == std::string::npos) return;
		mString.replace(startPos, mFrom.size(), mTo);
	}

	/*!
	 *
	 * @brief Replace all occurrences of a string in a string with another string.
	 *
	 * @param mString String to work with.
	 * @param mFrom String to replace.
	 * @param mTo Replacement string.
	 *
	 */
	inline void replaceAll(std::string& mString, const std::string& mFrom, const std::string& mTo)
	{
		size_t startPos{mString.find(mFrom)};
		while(startPos != std::string::npos)
		{
			mString.replace(startPos, mFrom.size(), mTo);
			startPos = mString.find(mFrom);
		}
	}

	/*!
	 *
	 * @brief Returns whether a string starts with a string or not.
	 *
	 * @param mString String to work with.
	 * @param mStart String to test.
	 *
	 * @return Returns true if mString starts with mStart.
	 *
	 */
	inline bool beginsWith(const std::string& mString, const std::string& mStart) { return mStart.size() <= mString.size() && mString.compare(0, mStart.size(), mStart) == 0; }

	// TODO: docs
	inline bool beginsWith(const std::string& mString, char mChar) { return mString.size() > 0 && mString.front() == mChar; }

	/*!
	 *
	 * @brief Returns whether a string ends with a string or not.
	 *
	 * @param mString String to work with.
	 * @param mEnding String to test.
	 *
	 * @return Returns true if mString ends with mEnding.
	 *
	 */
	inline bool endsWith(const std::string &mString, const std::string &mEnding) { return mString.size() >= mEnding.size() && mString.compare(mString.size() - mEnding.size(), mEnding.size(), mEnding) == 0; }

	// TODO: docs
	inline bool endsWith(const std::string& mString, char mChar) { return mString.size() > 0 && mString.back() == mChar; }

	/*!
	 *
	 * @brief Converts a string to a lowercase string.
	 *
	 * @param mString String to convert.
	 *
	 * @return Returns a std::string that is a copy of mString, with all characters lowercase.
	 *
	 */
	inline std::string toLower(std::string mString) { std::transform(std::begin(mString), std::end(mString), std::begin(mString), ::tolower); return mString; }

	/*!
	 *
	 * @brief Copies a string and replaces the first occurrence of a string in it with another string.
	 *
	 * @param mString String to copy.
	 * @param mFrom String to replace.
	 * @param mTo Replacement string.
	 *
	 * @return Returns a std::string with mFrom replaced by mTo (first occurrence).
	 *
	 */
	inline std::string getReplaced(std::string mString, const std::string& mFrom, const std::string& mTo) { replace(mString, mFrom, mTo); return mString; }

	/*!
	 *
	 * @brief Copies a string and replaces the all occurences of a string in it with another string.
	 *
	 * @param mString String to copy.
	 * @param mFrom String to replace.
	 * @param mTo Replacement string.
	 *
	 * @return Returns a std::string with mFrom replaced by mTo (all occurrences).
	 *
	 */
	inline std::string getReplacedAll(std::string mString, const std::string& mFrom, const std::string& mTo) { replaceAll(mString, mFrom, mTo); return mString; }

	/*!
	 *
	 * @brief Counts the number of occurences of a certain char in a std::string.
	 *
	 * @param mString String to work with.
	 * @param mChar Character to look for.
	 *
	 * @return Returns an unsigned int which is the count of the character's occurrences.
	 *
	 */
	inline std::size_t getCharCount(const std::string& mString, const char& mChar) { return std::count(std::begin(mString), std::end(mString), mChar); }

	/*!
	 *
	 * @brief Counts the number of newlines in a std::string.
	 *
	 * @param mString String to work with.
	 *
	 * @return Returns an unsigned int which is the count of newlines.
	 *
	 */
	inline std::size_t getNewLinesCount(const std::string& mString) { return getCharCount(mString, '\n'); }

	/*!
	 *
	 * @brief Calculates the levenshtein distance between two strings.
	 *
	 * Levenshtein distance measures the "difference" between two strings.
	 * Useful for mispelling detection, for example.
	 *
	 * @param mA First string.
	 * @param mB Second string.
	 *
	 * @return Returns the levenshtein distance between two strings as a size_t.
	 *
	 */
	inline size_t getLevenshteinDistance(const std::string& mA, const std::string& mB)
	{
		const auto& m(mA.size());
		const auto& n(mB.size());
		if(m == 0) return n;
		if(n == 0) return m;

		auto costs(Uptr<size_t[]>(new size_t[n + 1]));
		for(size_t k{0}; k <= n; ++k) costs[k] = k;

		size_t i{0};
		for(auto it1(std::begin(mA)); it1 != std::end(mA); ++it1, ++i)
		{
			costs[0] = i + 1;
			auto corner(i);

			size_t j{0};
			for(auto it2(std::begin(mB)); it2 != std::end(mB); ++it2, ++j)
			{
				auto upper(costs[j + 1]);
				if(*it1 == *it2) costs[j + 1] = corner;
				else
				{
					const auto& t(upper < corner ? upper : corner);
					costs[j + 1] = (costs[j] < t ? costs[j] : t) + 1;
				}

				corner = upper;
			}
		}
		return costs[n];
	}
}

#endif

