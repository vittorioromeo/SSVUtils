// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_COMMANDLINE_ELEMENTS_FLAG
#define SSVU_COMMANDLINE_ELEMENTS_FLAG

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
				inline Flag(std::string mShortName, std::string mLongName) noexcept : shortName{std::move(mShortName)}, longName{std::move(mLongName)} { }

				inline Flag& operator=(bool mActive) noexcept	{ active = mActive; return *this; }
				inline operator bool() const noexcept			{ return active; }

				inline const auto& getShortName() const noexcept				{ return shortName; }
				inline const auto& getLongName() const noexcept					{ return longName; }
				inline auto getShortNameWithPrefix() const noexcept				{ return std::string{flagPrefixShort + shortName}; }
				inline auto getLongNameWithPrefix() const noexcept				{ return std::string{flagPrefixLong + longName}; }
				inline bool hasName(const std::string& mName) const noexcept	{ return mName == getShortNameWithPrefix() || mName == getLongNameWithPrefix(); }
				inline std::string getUsageStr() const override					{ return "[" + getShortNameWithPrefix() + " || " + getLongNameWithPrefix() + "]"; }
		};
	}
}

#endif
