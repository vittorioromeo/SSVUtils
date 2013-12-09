// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_STRING_UTILS
#define SSVU_STRING_UTILS

#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include "SSVUtils/Global/Common.hpp"
#include "SSVUtils/Utils/UtilsContainers.hpp"
#include "SSVUtils/Utils/UtilsIterator.hpp"

namespace ssvu
{
	template<typename> struct Stringifier;

	namespace Internal
	{
		void resetFmt(std::ostream& mStream);
		inline std::ostringstream& getStringifyStream() noexcept { static std::ostringstream oss; return oss; }
		template<bool TFmt, bool TResetFmt = true, typename T> inline void callStringifyImpl(std::ostream& mStream, const T& mValue)
		{
			if(TResetFmt) resetFmt(mStream);
			Stringifier<T>::template impl<TFmt>(mStream, mValue);
			if(TResetFmt) resetFmt(mStream);
		}
	}

	/// @brief Stringifies a value.
	/// @details Uses Stringifier<T> internally.
	/// @param mValue Const reference to the value. (original value won't be changed)
	/// @return Returns a std::string representing the stringified value.
	template<typename T> inline std::string toStr(const T& mValue)
	{
		Internal::getStringifyStream().str("");
		Internal::callStringifyImpl<false, false>(Internal::getStringifyStream(), mValue);
		return Internal::getStringifyStream().str();
	}
}

#include "SSVUtils/Utils/UtilsConsole.hpp"

// TODO: docs
namespace ssvu
{
	template<typename T> struct Stringifier
	{
		template<bool TFmt> inline static void impl(std::ostream& mStream, const T& mValue) { mStream << mValue; }
	};

	namespace Internal
	{
		inline void resetFmt(std::ostream& mStream) { mStream << Console::resetFmt(); }
	}

	#define SSVU_DEFINE_FLAVORED_STRINGIFIER(mType, mColor, mStyle, mPostfix) \
		template<> struct Stringifier<mType> \
		{ \
			template<bool TFmt> inline static void impl(std::ostream& mStream, const mType& mValue) \
			{ \
				if(TFmt) mStream << Console::setStyle(mStyle) << Console::setColorFG(mColor); \
				mStream << mValue; \
				if(TFmt) mStream << mPostfix; \
			} \
		} \

	SSVU_DEFINE_FLAVORED_STRINGIFIER(int,				Console::Color::LightBlue,		Console::Style::ReverseFGBG,	"");
	SSVU_DEFINE_FLAVORED_STRINGIFIER(long,				Console::Color::LightBlue,		Console::Style::ReverseFGBG,	"l");
	SSVU_DEFINE_FLAVORED_STRINGIFIER(unsigned int,		Console::Color::LightBlue,		Console::Style::ReverseFGBG,	"u");
	SSVU_DEFINE_FLAVORED_STRINGIFIER(unsigned long,		Console::Color::LightBlue,		Console::Style::ReverseFGBG,	"ul");
	SSVU_DEFINE_FLAVORED_STRINGIFIER(float,				Console::Color::LightRed,		Console::Style::ReverseFGBG,	"f");
	SSVU_DEFINE_FLAVORED_STRINGIFIER(double,			Console::Color::LightRed,		Console::Style::ReverseFGBG,	"d");
	SSVU_DEFINE_FLAVORED_STRINGIFIER(std::string,		Console::Color::LightYellow,	Console::Style::Underline,		"");

	#undef SSVU_DEFINE_FLAVORED_STRINGIFIER

	namespace Internal
	{
		template<bool TFmt, bool TResetFmt = true, typename T> inline void printFmt(std::ostream& mStream, const T& mValue,
			Console::Color mColor = Console::Color::Default, Console::Style mStyle = Console::Style::None)
		{
			if(TFmt)
			{
				if(TResetFmt) resetFmt(mStream);
				mStream << Console::setColorFG(mColor) << Console::setStyle(mStyle);
			}

			mStream << mValue;
		}

		template<bool TFmt, typename T> inline void printBold(std::ostream& mStream, const T& mValue, Console::Color mColor = Console::Color::Default)
		{
			printFmt<TFmt>(mStream, mValue, mColor, Console::Style::Bold);
		}
		template<bool TFmt, typename T> inline void printNonBold(std::ostream& mStream, const T& mValue, Console::Color mColor = Console::Color::Default)
		{
			printFmt<TFmt>(mStream, mValue, mColor, Console::Style::None);
		}

		template<bool TFmt, std::size_t I = 0, typename... TArgs> inline EnableIf<I == sizeof...(TArgs), void> implTpl(std::ostream&, const std::tuple<TArgs...>&) { }
		template<bool TFmt, std::size_t I = 0, typename... TArgs> inline EnableIf<I < sizeof...(TArgs), void> implTpl(std::ostream& mStream, const std::tuple<TArgs...>& mTpl)
		{
			callStringifyImpl<TFmt>(mStream, std::get<I>(mTpl));
			if(I < sizeof...(TArgs) - 1) printBold<TFmt>(mStream, ", ");
			implTpl<TFmt, I + 1, TArgs...>(mStream, mTpl);
		}

		template<bool TFmt, typename T> inline void implContainer(std::ostream& mStream, const T& mValue)
		{
			printBold<TFmt>(mStream, "{");
			for(auto itr(std::begin(mValue)); itr < std::end(mValue) - 1; ++itr)
			{
				callStringifyImpl<TFmt>(mStream, *itr);
				printBold<TFmt>(mStream, ", ");
			}
			callStringifyImpl<TFmt>(mStream, *(std::end(mValue) - 1));
			printBold<TFmt>(mStream, "}");
		}
	}

	// Stringify pair
	template<typename T1, typename T2> struct Stringifier<std::pair<T1, T2>>
	{
		template<bool TFmt> inline static void impl(std::ostream& mStream, const std::pair<T1, T2>& mValue)
		{
			Internal::printBold<TFmt>(mStream, "{");
			Internal::callStringifyImpl<TFmt>(mStream, std::get<0>(mValue));
			Internal::printBold<TFmt>(mStream, ", ");
			Internal::callStringifyImpl<TFmt>(mStream, std::get<1>(mValue));
			Internal::printBold<TFmt>(mStream, "}");
		}
	};

	// Stringify tuple
	template<typename... TArgs> struct Stringifier<std::tuple<TArgs...>>
	{
		template<bool TFmt> inline static void impl(std::ostream& mStream, const std::tuple<TArgs...>& mValue)
		{
			Internal::printBold<TFmt>(mStream, "{");
			Internal::implTpl<TFmt>(mValue, mStream);
			Internal::printBold<TFmt>(mStream, "}");
		}
	};

	// Stringify pointer
	template<typename T> struct Stringifier<T*>
	{
		template<bool TFmt> inline static void impl(std::ostream& mStream, const T* mValue)
		{
			Internal::printBold<TFmt>(mStream, "[", Console::Color::Blue);
			Internal::printFmt<TFmt>(mStream, mValue != nullptr ? reinterpret_cast<const void*>(mValue) : "nullptr", Console::Color::Cyan, Console::Style::Underline);
			Internal::printBold<TFmt>(mStream, "]", Console::Color::Blue);
		}
	};

	// Stringify C-style string
	template<std::size_t TN> struct Stringifier<char[TN]>
	{
		template<bool TFmt> inline static void impl(std::ostream& mStream, const char(&mValue)[TN]) { mStream << mValue; }
	};

	// Stringify array
	template<typename T, std::size_t TN> struct Stringifier<T[TN]>
	{
		template<bool TFmt> inline static void impl(std::ostream& mStream, const T(&mValue)[TN]) { Internal::implContainer<TFmt>(mStream, mValue); }
	};

	// Stringify linear containers
	template<template<typename, typename...> class T, typename TType, typename... TArgs> struct Stringifier<T<TType, TArgs...>>
	{
		template<bool TFmt> inline static void impl(std::ostream& mStream, const T<TType, TArgs...>& mValue) { Internal::implContainer<TFmt>(mStream, mValue); }
	};

	/// @brief Replace the first occurrence of a string in a string with another string.
	/// @param mStr String to work with.
	/// @param mFrom String to replace.
	/// @param mTo Replacement string.
	inline void replace(std::string& mStr, const std::string& mFrom, const std::string& mTo)
	{
		std::size_t startPos{mStr.find(mFrom)};
		if(startPos == std::string::npos) return;
		mStr.replace(startPos, mFrom.size(), mTo);
	}

	/// @brief Replace all occurrences of a string in a string with another string.
	/// @param mStr String to work with.
	/// @param mFrom String to replace.
	/// @param mTo Replacement string.
	inline void replaceAll(std::string& mStr, const std::string& mFrom, const std::string& mTo)
	{
		std::size_t startPos{mStr.find(mFrom)};
		while(startPos != std::string::npos)
		{
			mStr.replace(startPos, mFrom.size(), mTo);
			startPos = mStr.find(mFrom);
		}
	}

	/// @brief Returns whether a string starts with a specific string.
	/// @param mStr String to work with.
	/// @param mStart String to test.
	/// @return Returns true if mStr starts with mStart.
	inline bool beginsWith(const std::string& mStr, const std::string& mStart) noexcept { return mStart.size() <= mStr.size() && mStr.compare(0, mStart.size(), mStart) == 0; }

	/// @brief Returns whether a string starts with a specific char.
	/// @param mStr String to work with.
	/// @param mChar Char to test.
	/// @return Returns true if mStr starts with mChar.
	inline bool beginsWith(const std::string& mStr, char mChar) noexcept { return mStr.size() > 0 && mStr.front() == mChar; }

	/// @brief Returns whether a string ends with a specific string.
	/// @param mStr String to work with.
	/// @param mEnding String to test.
	/// @return Returns true if mStr ends with mEnding.
	inline bool endsWith(const std::string& mStr, const std::string& mEnding) noexcept { return mStr.size() >= mEnding.size() && mStr.compare(mStr.size() - mEnding.size(), mEnding.size(), mEnding) == 0; }

	/// @brief Returns whether a string ends with a specific char.
	/// @param mStr String to work with.
	/// @param mChar Char to test.
	/// @return Returns true if mStr ends with mChar.
	inline bool endsWith(const std::string& mStr, char mChar) noexcept { return mStr.size() > 0 && mStr.back() == mChar; }

	/// @brief Converts a string to a lowercase string.
	/// @param mStr String to convert.
	/// @return Returns a std::string that is a copy of mStr, with all characters lowercase.
	inline std::string toLower(std::string mStr) { std::transform(std::begin(mStr), std::end(mStr), std::begin(mStr), ::tolower); return mStr; }

	/// @brief Converts a string to an uppercase string.
	/// @param mStr String to convert.
	/// @return Returns a std::string that is a copy of mStr, with all characters uppercase.
	inline std::string toUpper(std::string mStr) { std::transform(std::begin(mStr), std::end(mStr), std::begin(mStr), ::toupper); return mStr; }

	/// @brief Copies a string and replaces the first occurrence of a string in it with another string.
	/// @param mStr String to copy.
	/// @param mFrom String to replace.
	/// @param mTo Replacement string.
	/// @return Returns a std::string with mFrom replaced by mTo (first occurrence).
	inline std::string getReplaced(std::string mStr, const std::string& mFrom, const std::string& mTo) { replace(mStr, mFrom, mTo); return mStr; }

	/// @brief Copies a string and replaces the all occurences of a string in it with another string.
	/// @param mStr String to copy.
	/// @param mFrom String to replace.
	/// @param mTo Replacement string.
	/// @return Returns a std::string with mFrom replaced by mTo (all occurrences).
	inline std::string getReplacedAll(std::string mStr, const std::string& mFrom, const std::string& mTo) { replaceAll(mStr, mFrom, mTo); return mStr; }

	/// @brief Counts the number of occurences of a certain char in a std::string.
	/// @param mStr String to work with.
	/// @param mChar Character to look for.
	/// @return Returns an unsigned int which is the count of the character's occurrences.
	inline std::size_t getCharCount(const std::string& mStr, const char& mChar) { return std::count(std::begin(mStr), std::end(mStr), mChar); }

	/// @brief Counts the number of newlines in a std::string.
	/// @param mStr String to work with.
	/// @return Returns an unsigned int which is the count of newlines.
	inline std::size_t getNewLinesCount(const std::string& mStr) { return getCharCount(mStr, '\n'); }

	/// @brief Calculates the levenshtein distance between two strings.
	/// @details Levenshtein distance measures the "difference" between two strings.
	/// Useful for mispelling detection, for example.
	/// @param mA First string.
	/// @param mB Second string.
	/// @return Returns the levenshtein distance between two strings as an std::size_t.
	inline std::size_t getLevenshteinDistance(const std::string& mA, const std::string& mB)
	{
		const auto& m(mA.size());
		const auto& n(mB.size());
		if(m == 0) return n;
		if(n == 0) return m;

		auto costs(Uptr<std::size_t[]>(new std::size_t[n + 1]));
		for(std::size_t k{0}; k <= n; ++k) costs[k] = k;

		std::size_t i{0};
		for(auto it1(std::begin(mA)); it1 != std::end(mA); ++it1, ++i)
		{
			costs[0] = i + 1;
			auto corner(i);

			std::size_t j{0};
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
	inline std::string getTrimmedStrL(std::string mStr, char mChar = ' ') { trimStrL(mStr, mChar); return mStr; }

	/// @brief Gets a copy of a string with characters of a certain type trimmed from the right.
	/// @param mStr String to trim. (will not be modified)
	/// @param mChar Character to trim.
	/// @return Returns a copy of the string, trimmed.
	inline std::string getTrimmedStrR(std::string mStr, char mChar = ' ') { trimStrR(mStr, mChar); return mStr; }

	/// @brief Gets a copy of a string with characters of a certain type trimmed both from the left and right.
	/// @param mStr String to trim. (will not be modified)
	/// @param mChar Character to trim.
	/// @return Returns a copy of the string, trimmed.
	inline std::string getTrimmedStrLR(std::string mStr, char mChar = ' ') { trimStrLR(mStr, mChar); return mStr; }
}

#endif
