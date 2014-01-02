// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_STRING_STRINGIFIER
#define SSVU_STRING_STRINGIFIER

#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include "SSVUtils/Global/Common.hpp"
#include "SSVUtils/String/Utils.hpp"
#include "SSVUtils/Utils/Containers.hpp"
#include "SSVUtils/Utils/Iterator.hpp"
#include "SSVUtils/Utils/Console.hpp"

// TODO: docs
namespace ssvu
{
	namespace Internal
	{
		inline void resetFmt(std::ostream& mStream) { mStream << Console::resetFmt(); }

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

		template<bool TFmt, std::size_t I = 0, typename... TArgs, template<typename...> class T> inline EnableIf<I == sizeof...(TArgs), void> implTpl(std::ostream&, const T<TArgs...>&) { }
		template<bool TFmt, std::size_t I = 0, typename... TArgs, template<typename...> class T> inline EnableIf<I < sizeof...(TArgs), void> implTpl(std::ostream& mStream, const T<TArgs...>& mTpl)
		{
			callStringifyImpl<TFmt>(mStream, std::get<I>(mTpl));
			if(I < sizeof...(TArgs) - 1) printBold<TFmt>(mStream, ", ");
			implTpl<TFmt, I + 1>(mStream, mTpl);
		}

		template<typename T> struct StringifierDefault
		{
			template<bool TFmt> inline static void impl(std::ostream& mStream, const T& mValue) { mStream << mValue; }
		};
		template<typename T> struct StringifierContainer
		{
			template<bool TFmt> inline static void impl(std::ostream& mStream, const T& mValue)
			{
				if(std::begin(mValue) == std::end(mValue)) { printBold<TFmt>(mStream, "{ EMPTY }"); return; }

				printBold<TFmt>(mStream, "{");
				for(auto itr(std::begin(mValue)); itr < std::end(mValue) - 1; ++itr)
				{
					callStringifyImpl<TFmt>(mStream, *itr);
					printBold<TFmt>(mStream, ", ");
				}
				callStringifyImpl<TFmt>(mStream, *(std::end(mValue) - 1));
				printBold<TFmt>(mStream, "}");
			}
		};
		template<typename T> struct StringifierTuple
		{
			template<bool TFmt> inline static void impl(std::ostream& mStream, const T& mValue)
			{
				printBold<TFmt>(mStream, "{");
				implTpl<TFmt>(mStream, mValue);
				printBold<TFmt>(mStream, "}");
			}
		};
	}

	// Stringify common types
	template<typename T> struct Stringifier : public Internal::StringifierDefault<T> { };

	// Stringify C-style strings
	template<std::size_t TN> struct Stringifier<char[TN]> : public Internal::StringifierDefault<char[TN]> { };
	template<> struct Stringifier<char*> : public Internal::StringifierDefault<char*> { };
	template<> struct Stringifier<const char*> : public Internal::StringifierDefault<const char*> { };

	// Stringify flavored types
	#define SSVU_DEFINE_FLAVORED_STRINGIFIER(mType, mColor, mStyle, mPostfix) \
		template<> struct Stringifier<mType> \
		{ \
			template<bool TFmt> inline static void impl(std::ostream& mStream, const mType& mValue) \
			{ \
				if(TFmt) mStream << Console::setStyle(mStyle) << Console::setColorFG(mColor); \
				mStream << mValue; \
				if(TFmt) mStream << mPostfix; \
			} \
		}
	SSVU_DEFINE_FLAVORED_STRINGIFIER(int,				Console::Color::LightBlue,		Console::Style::Bold,		"");
	SSVU_DEFINE_FLAVORED_STRINGIFIER(long,				Console::Color::LightBlue,		Console::Style::Bold,		"l");
	SSVU_DEFINE_FLAVORED_STRINGIFIER(unsigned int,		Console::Color::LightBlue,		Console::Style::Bold,		"u");
	SSVU_DEFINE_FLAVORED_STRINGIFIER(unsigned long,		Console::Color::LightBlue,		Console::Style::Bold,		"ul");
	SSVU_DEFINE_FLAVORED_STRINGIFIER(float,				Console::Color::LightRed,		Console::Style::Bold,		"f");
	SSVU_DEFINE_FLAVORED_STRINGIFIER(double,			Console::Color::LightRed,		Console::Style::Bold,		"d");
	SSVU_DEFINE_FLAVORED_STRINGIFIER(std::string,		Console::Color::LightYellow,	Console::Style::Underline,	"");
	#undef SSVU_DEFINE_FLAVORED_STRINGIFIER

	// Stringify tuples
	template<typename T1, typename T2> struct Stringifier<std::pair<T1, T2>> : public Internal::StringifierTuple<std::pair<T1, T2>> { };
	template<typename... TArgs> struct Stringifier<std::tuple<TArgs...>> : public Internal::StringifierTuple<std::tuple<TArgs...>> { };

	// Stringify containers
	template<typename T, std::size_t TN> struct Stringifier<T[TN]> : public Internal::StringifierContainer<T[TN]> { };
	template<template<typename, typename...> class T, typename TV, typename... TArgs> struct Stringifier<T<TV, TArgs...>> : public Internal::StringifierContainer<T<TV, TArgs...>> { };

	// Stringify pointer
	template<typename T> struct Stringifier<T*>
	{
		template<bool TFmt> inline static void impl(std::ostream& mStream, const T* mValue, EnableIf<!isSame<RemoveConst<T>, char>()>* = nullptr)
		{
			Internal::printBold<TFmt>(mStream, "[", Console::Color::Blue);
			Internal::printFmt<TFmt>(mStream, mValue != nullptr ? reinterpret_cast<const void*>(mValue) : "nullptr", Console::Color::Cyan, Console::Style::Underline);
			Internal::printBold<TFmt>(mStream, "]", Console::Color::Blue);
		}
	};
}

#endif
