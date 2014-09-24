// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_CMDLINE_ELEMENTS_BASES
#define SSVU_CMDLINE_ELEMENTS_BASES

namespace ssvu
{
	namespace CmdLine
	{
		class ElementBase
		{
			private:
				std::string name, briefDesc, desc;

			public:
				inline virtual ~ElementBase() { }

				inline void setName(std::string mName)				{ name = std::move(mName); }
				inline void setBriefDesc(std::string mBriefDesc)	{ briefDesc = std::move(mBriefDesc); }
				inline void setDesc(std::string mDesc)				{ desc = std::move(mDesc); }

				inline const auto& getName() const noexcept			{ return name; }
				inline const auto& getBriefDesc() const noexcept	{ return briefDesc; }
				inline const auto& getDesc() const noexcept			{ return desc; }
				inline virtual std::string getUsageStr() const		{ return ""; }
				inline auto getHelpStr()
				{
					std::string result, usageStr{this->getUsageStr()};

					if(!usageStr.empty())	result += "* " + usageStr;
					if(!name.empty())		result += "\n  --" + name;
					if(!briefDesc.empty())	result += "\n  --" + briefDesc;
					if(!desc.empty())		result += "\n  --" + desc;

					return result + "\n\n";
				}
		};

		struct ArgBase : public ElementBase
		{
			virtual void set(const std::string& mStr) = 0;
			inline std::string getUsageStr() const override { return "(ARG " + getName() + ")"; }
		};

		class ArgPackBase : public ElementBase
		{
			protected:
				std::size_t min, max;

			public:
				inline ArgPackBase() noexcept : min{0}, max{0} { }
				inline ArgPackBase(std::size_t mMin, std::size_t mMax) noexcept : min{mMin}, max{mMax} { }

				virtual void set(const std::vector<std::string>& mStrings) = 0;

				inline bool isInfinite() const noexcept { return min == 0 && max == 0; }
				inline auto getMin() const noexcept { return min; }
				inline auto getMax() const noexcept { return max; }
				inline std::string getUsageStr() const override
				{
					return "(PACK " + getName() + " " + "[" + toStr(min) + "/" + (isInfinite() ? "..." : toStr(max)) + "])";
				}
		};

		constexpr const char* flagPrefixShort{"-"};
		constexpr const char* flagPrefixLong{"--"};

		class FlagBase : public ElementBase
		{
			private:
				std::string shortName, longName;

			public:
				inline FlagBase(std::string mShortName, std::string mLongName) noexcept : shortName{std::move(mShortName)}, longName{std::move(mLongName)} { }

				inline const auto& getShortName() const noexcept				{ return shortName; }
				inline const auto& getLongName() const noexcept					{ return longName; }
				inline auto getShortNameWithPrefix() const noexcept				{ return std::string{flagPrefixShort + shortName}; }
				inline auto getLongNameWithPrefix() const noexcept				{ return std::string{flagPrefixLong + longName}; }
				inline bool hasName(const std::string& mName) const noexcept	{ return mName == getShortNameWithPrefix() || mName == getLongNameWithPrefix(); }
		};

		struct FlagValueBase : public FlagBase
		{
			inline FlagValueBase(std::string mShortName, std::string mLongName) noexcept : FlagBase{std::move(mShortName), std::move(mLongName)} { }
			virtual void set(const std::string& mStr) = 0;
		};
	}
}

#endif
