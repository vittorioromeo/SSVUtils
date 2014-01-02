// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_COMMANDLINE_ELEMENTS_FLAG
#define SSVU_COMMANDLINE_ELEMENTS_FLAG

#include <string>

namespace ssvu
{
	namespace CommandLine
	{
		constexpr const char* flagPrefixShort{"-"};
		constexpr const char* flagPrefixLong{"--"};

		class Flag final : public ElementBase
		{
			private:
				std::string shortName, longName;
				bool active{false};

			public:
				Flag(std::string mShortName, std::string mLongName) noexcept : shortName{std::move(mShortName)}, longName{std::move(mLongName)} { }

				inline Flag& operator=(bool mActive) noexcept	{ active = mActive; return *this; }
				inline operator bool() const noexcept			{ return active; }

				inline const std::string& getShortName() const noexcept			{ return shortName; }
				inline const std::string& getLongName() const noexcept			{ return longName; }
				inline std::string getShortNameWithPrefix() const noexcept		{ return flagPrefixShort + shortName; }
				inline std::string getLongNameWithPrefix() const noexcept		{ return flagPrefixLong + longName; }
				inline bool hasName(const std::string& mName) const noexcept	{ return mName == getShortNameWithPrefix() || mName == getLongNameWithPrefix(); }
				inline std::string getUsageStr() const override					{ return "[" + getShortNameWithPrefix() + " || " + getLongNameWithPrefix() + "]"; }
		};
	}
}

#endif
