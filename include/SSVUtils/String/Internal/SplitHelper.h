// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_STRING_INTERNAL_SPLITHELPER
#define SSVU_STRING_INTERNAL_SPLITHELPER

#include <vector>
#include <string>
#include "SSVUtils/String/Enums.h"

namespace ssvu
{
	namespace Internal
	{
		using StringSize = std::string::size_type;

		StringSize findFirstOf(const std::string& mString, const std::vector<std::string>& mSeparator, StringSize& mLength);

		template<typename T> struct SplitFindHelper;
		template<> struct SplitFindHelper<char>
		{
			inline static StringSize getNextIndex(const std::string& mString, const char& mSeparator, StringSize mStart)
			{
				return mString.find(mSeparator, mStart);
			}
		};
		template<> struct SplitFindHelper<std::vector<char>>
		{
			inline static StringSize getNextIndex(const std::string& mString, const std::vector<char>& mSeparator, StringSize mStart)
			{
				return mString.find_first_of(std::string{std::begin(mSeparator), std::end(mSeparator)}, mStart);
			}
		};

		template<typename T, SplitMode TM> struct SplitHelper
		{
			inline static void split(std::vector<std::string>& mTarget, const std::string& mString, const T& mSeparator)
			{
				StringSize p{0}, q;
				while ((q = SplitFindHelper<T>::getNextIndex(mString, mSeparator, p)) != std::string::npos)
				{
					auto tokenLength(q - p);

					// If we need to keep the separator in the splitted strings, add 1 to the token length (assuming a char has always length 1)
					if(TM == SplitMode::KeepSeparator) tokenLength += 1;

					std::string token{mString, p, tokenLength};
					if(!token.empty()) mTarget.push_back(token);
					p = q + 1;
				}
				std::string remaining{mString, p};
				if(!remaining.empty()) mTarget.push_back(remaining);
			}
		};
		template<SplitMode TM> struct SplitHelper<std::string, TM>
		{
			inline static void split(std::vector<std::string>& mTarget, std::string mString, const std::string& mSeparator)
			{
				StringSize pos{0};
				std::string token;
				while ((pos = mString.find(mSeparator)) != std::string::npos)
				{
					auto tokenLength(pos);

					// If we need to keep the separator in the splitted strings, add the separator's length to the token length
					if(TM == SplitMode::KeepSeparator) tokenLength += mSeparator.length();

					token = mString.substr(0, tokenLength);
					if(!token.empty()) mTarget.push_back(token);
					mString.erase(0, pos + mSeparator.length());
				}
				if(!mString.empty()) mTarget.push_back(mString);
			}
		};
		template<SplitMode TM> struct SplitHelper<std::vector<std::string>, TM>
		{
			inline static void split(std::vector<std::string>& mTarget, std::string mString, const std::vector<std::string>& mSeparator)
			{
				StringSize pos{0}, lastLength;
				std::string token;
				while ((pos = findFirstOf(mString, mSeparator, lastLength)) != std::string::npos)
				{
					auto tokenLength(pos);

					// If we need to keep the separator in the splitted strings, add the separator's length to the token length
					if(TM == SplitMode::KeepSeparator) tokenLength += lastLength;

					token = mString.substr(0, tokenLength);
					if(!token.empty()) mTarget.push_back(token);
					mString.erase(0, pos + lastLength);
				}
				if(!mString.empty()) mTarget.push_back(mString);
			}
		};
	}
}

#endif
