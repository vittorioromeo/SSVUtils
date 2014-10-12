// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_CORE_STRING_INTERNAL_SPLITHELPER
#define SSVU_CORE_STRING_INTERNAL_SPLITHELPER

namespace ssvu
{
	namespace Internal
	{
		using StringSize = std::string::size_type;

		inline StringSize findFirstOf(const std::string& mStr, const StringSize& mStartAt, const std::vector<std::string>& mSeparator, StringSize& mLength)
		{
			StringSize result{std::numeric_limits<StringSize>::max()};
			for(const auto& s : mSeparator)
			{
				StringSize temp{mStr.find(s, mStartAt)};
				if(temp < result) { result = temp; mLength = s.size(); }
			}
			return result;
		}

		template<typename T> struct SplitFindHelperDefault
		{
			inline static StringSize getNextIdx(const std::string& mStr, const T& mSeparator, StringSize mStart)
			{
				return mStr.find(mSeparator, mStart);
			}
		};

		template<typename T> struct SplitFindHelper;
		template<> struct SplitFindHelper<char> : public SplitFindHelperDefault<char> { };
		template<> struct SplitFindHelper<std::string> : public SplitFindHelperDefault<std::string> { };
		template<SizeT TN> struct SplitFindHelper<char[TN]>
		{
			inline static StringSize getNextIdx(const std::string& mStr, const char(&mSeparator)[TN], StringSize mStart)
			{
				std::string separator(&mSeparator[0], &mSeparator[TN - 1]);
				return mStr.find(separator, mStart);
			}
		};

		template<typename T, Split TM> struct SplitHelperImpl
		{
			inline static void split(std::vector<std::string>& mTarget, const std::string& mStr, const T& mSeparator, SizeT mSeparatorSize)
			{
				StringSize pos{0}, startAt{0};
				std::string token;

				while((pos = mStr.find(mSeparator, startAt)) != std::string::npos)
				{
					auto tokenLength(pos - startAt);

					// If we need to keep the separator in the splitted strings, add the separator's length to the token length
					if(TM == Split::TrailingSeparator) tokenLength += mSeparatorSize;

					token = mStr.substr(startAt, tokenLength);
					if(!token.empty()) mTarget.emplace_back(token);
					if(TM == Split::TokenizeSeparator) mTarget.emplace_back(mStr, pos - 1, mSeparatorSize);
					startAt = pos + mSeparatorSize;
				}

				std::string reimaining(mStr.substr(startAt, mStr.size() - startAt));
				if(!reimaining.empty()) mTarget.emplace_back(reimaining);
			}
		};

		template<typename T, Split TM> struct SplitHelper;
		template<Split TM> struct SplitHelper<char, TM>
		{
			inline static void split(std::vector<std::string>& mTarget, const std::string& mStr, char mSeparator)
			{
				SplitHelperImpl<char, TM>::split(mTarget, mStr, mSeparator, 1);
			}
		};
		template<Split TM, SizeT TN> struct SplitHelper<char[TN], TM>
		{
			inline static void split(std::vector<std::string>& mTarget, const std::string& mStr, const char(&mSeparator)[TN])
			{
				SplitHelperImpl<char[TN], TM>::split(mTarget, mStr, mSeparator, TN - 1);
			}
		};
		template<Split TM> struct SplitHelper<std::string, TM> : public SplitHelperImpl<std::string, TM>
		{
			inline static void split(std::vector<std::string>& mTarget, const std::string& mStr, const std::string& mSeparator)
			{
				SplitHelperImpl<std::string, TM>::split(mTarget, mStr, mSeparator, mSeparator.size());
			}
		};
		template<Split TM> struct SplitHelper<std::vector<std::string>, TM>
		{
			inline static void split(std::vector<std::string>& mTarget, const std::string& mStr, const std::vector<std::string>& mSeparator)
			{
				StringSize pos{0}, startAt{0}, lastLength;
				std::string token;
				while((pos = findFirstOf(mStr, startAt, mSeparator, lastLength)) != std::string::npos)
				{
					auto tokenLength(pos - startAt);

					// If we need to keep the separator in the splitted strings, add the separator's length to the token length
					if(TM == Split::TrailingSeparator) tokenLength += lastLength;

					token = mStr.substr(startAt, tokenLength);
					if(!token.empty()) mTarget.emplace_back(token);
					if(TM == Split::TokenizeSeparator) mTarget.emplace_back(mStr, pos, lastLength);
					startAt = pos + lastLength;
				}

				std::string reimaining(mStr.substr(startAt, mStr.size() - startAt));
				if(!reimaining.empty()) mTarget.emplace_back(reimaining);
			}
		};
	}
}

#endif
