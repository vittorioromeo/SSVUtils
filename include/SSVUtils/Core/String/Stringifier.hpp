// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_CORE_STRING_STRINGIFIER
#define SSVU_CORE_STRING_STRINGIFIER

#include <sstream>
#include <string>
#include <vector>
#include <array>
#include <algorithm>
#include "SSVUtils/Core/Global/Common.hpp"
#include "SSVUtils/Core/String/Utils.hpp"
#include "SSVUtils/Core/Utils/Containers.hpp"
#include "SSVUtils/Core/ConsoleFmt/ConsoleFmt.hpp"

// TODO: move to bimap
// #include "SSVUtils/Bimap/Bimap.hpp"

namespace ssvu
{
	namespace Internal
	{
		/// @brief Streams a "formatting reset sequence" to mStream.
		inline void resetFmt(std::ostream& mStream) { mStream << Console::resetFmt(); }

		/// @brief Prints a value to a stream, with optional formatting parameters.
		/// @tparam TFmt If true, applies formatting.
		/// @tparam TResetFmt If true, resets current formatting before applying new one.
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

		/// @brief Shortcut to `printFmt`, using optional bold formatting.
		template<bool TFmt, typename T> inline void printBold(std::ostream& mStream, const T& mValue, Console::Color mColor = Console::Color::Default)
		{
			printFmt<TFmt>(mStream, mValue, mColor, Console::Style::Bold);
		}

		/// @brief Shortcut to `printFmt`, using optional non-bold formatting.
		template<bool TFmt, typename T> inline void printNonBold(std::ostream& mStream, const T& mValue, Console::Color mColor = Console::Color::Default)
		{
			printFmt<TFmt>(mStream, mValue, mColor, Console::Style::None);
		}

		/// @brief Tuple-printing implementation. (base of the recursion)
		template<bool TFmt, std::size_t I = 0, typename... TArgs, template<typename...> class T> inline EnableIf<I == sizeof...(TArgs)> implTpl(std::ostream&, const T<TArgs...>&) { }

		/// @brief Tuple-printing implementation. (recursive step)
		template<bool TFmt, std::size_t I = 0, typename... TArgs, template<typename...> class T> inline EnableIf<I < sizeof...(TArgs)> implTpl(std::ostream& mStream, const T<TArgs...>& mTpl)
		{
			callStringifyImpl<TFmt>(mStream, std::get<I>(mTpl));
			if(I < sizeof...(TArgs) - 1) printBold<TFmt>(mStream, ", ");
			implTpl<TFmt, I + 1>(mStream, mTpl);

		}

		/// @brief Utility function to avoid repetition. (implementation for BidirectionalIterator)
		template<typename TItr, typename TFunc1, typename TFunc2> inline void repeatPenultimateImpl(TItr mBegin, TItr mEnd, const TFunc1& mFunc, const TFunc2& mFuncSeparator, std::bidirectional_iterator_tag)
		{
			auto itrPenultimate(--mEnd);
			while(mBegin != itrPenultimate) { mFunc(*mBegin); mFuncSeparator(*mBegin); ++mBegin; }
			mFunc(*itrPenultimate);
		}

		/// @brief Utility function to avoid repetition. (implementation for ForwardIterator)
		template<typename TItr, typename TFunc1, typename TFunc2> inline void repeatPenultimateImpl(TItr mBegin, TItr mEnd, const TFunc1& mFunc, const TFunc2& mFuncSeparator, std::forward_iterator_tag)
		{
			auto count(std::distance(mBegin, mEnd));
			for(auto i(0u); i < count; ++i) { mFunc(*mBegin); if(i < count - 1) mFuncSeparator(*mBegin); ++mBegin; }
		}

		/// @brief Utility function to avoid repetition.
		/// @details Calls mFunc on each element, and mFuncSeparator on each element except the last. Dispatches `repeatPenultimateImpl`.
		template<typename TItr, typename TFunc1, typename TFunc2> inline void repeatPenultimate(TItr mBegin, TItr mEnd, const TFunc1& mFunc, const TFunc2& mFuncSeparator)
		{
			repeatPenultimateImpl(mBegin, mEnd, mFunc, mFuncSeparator, typename std::iterator_traits<TItr>::iterator_category());
		}

		/// @brief Default stringifier. Simply streams the value.
		template<typename T> struct StringifierDefault
		{
			template<bool TFmt> inline static void impl(std::ostream& mStream, const T& mValue) { mStream << mValue; }
		};

		/// @brief Linear container stringifier. Simply streams each element, separated by commas.
		template<typename T> struct StringifierContainer
		{
			template<bool TFmt> inline static void impl(std::ostream& mStream, const T& mValue)
			{
				auto itrBegin(std::begin(mValue));
				if(itrBegin == std::end(mValue)) { printBold<TFmt>(mStream, "{ EMPTY }"); return; }

				printBold<TFmt>(mStream, "{");

				// C++14: `auto` lambda
				repeatPenultimate(itrBegin, std::end(mValue), [&mStream](decltype(*itrBegin) mE){ callStringifyImpl<TFmt>(mStream, mE); }, [&mStream](decltype(*itrBegin)){ printBold<TFmt>(mStream, ", "); });

				printBold<TFmt>(mStream, "}");
			}
		};

		/// @brief Tuple stringifier. Calls `implTpl` to stringify the elements.
		template<typename T> struct StringifierTuple
		{
			template<bool TFmt> inline static void impl(std::ostream& mStream, const T& mValue)
			{
				printBold<TFmt>(mStream, "{");
				implTpl<TFmt>(mStream, mValue);
				printBold<TFmt>(mStream, "}");
			}
		};

		/// @brief Map stringifier. Streams each key <separator> value pair, separated by commas.
		template<bool TFmt, typename T, typename TSep> inline void stringifyMapImpl(std::ostream& mStream, const T& mValue, const TSep& mSeparator)
		{
			auto itrBegin(std::begin(mValue));
			if(std::begin(mValue) == std::end(mValue)) { printBold<TFmt>(mStream, "{ EMPTY }"); return; }

			printBold<TFmt>(mStream, "{");

			// C++14: `auto` lambda
			repeatPenultimate(itrBegin, std::end(mValue), [&mStream, &mSeparator](decltype(*itrBegin) mE)
			{
				callStringifyImpl<TFmt>(mStream, mE.first);
				printBold<TFmt>(mStream, mSeparator);
				callStringifyImpl<TFmt>(mStream, mE.second);
			}, [&mStream](decltype(*itrBegin)){ printBold<TFmt>(mStream, ", "); });

			printBold<TFmt>(mStream, "}");
		}
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

	// Stringify arrays
	template<typename T, std::size_t TN> struct Stringifier<T[TN]> : public Internal::StringifierContainer<T[TN]> { };
	template<typename T, std::size_t TN> struct Stringifier<std::array<T, TN>> : public Internal::StringifierContainer<std::array<T, TN>> { };

	// Stringify linear containers (value type and allocator type)
	template<template<typename, typename> class T, typename TV, typename TAlloc> struct Stringifier<T<TV, TAlloc>> : public Internal::StringifierContainer<T<TV, TAlloc>> { };

	// Stringify map-like containers
	template<template<typename, typename, typename, typename...> class TM, typename TK, typename TV, typename TComp, typename TAlloc, typename... TArgs> struct Stringifier<TM<TK, TV, TComp, TAlloc, TArgs...>>
	{
		template<bool TFmt> inline static void impl(std::ostream& mStream, const TM<TK, TV, TComp, TAlloc, TArgs...>& mValue)
		{
			Internal::stringifyMapImpl<TFmt>(mStream, mValue, " -> ");
		}
	};

	// TODO: move to bimap
	// Stringify bimap
	/*
	template<typename T1, typename T2, template<typename...> class TM> struct Stringifier<Bimap<T1, T2, TM>>
	{
		template<bool TFmt> inline static void impl(std::ostream& mStream, const Bimap<T1, T2, TM>& mValue)
		{
			Internal::stringifyMapImpl<TFmt>(mStream, mValue.getMap1(), " <-> ");
		}
	};
	*/

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
