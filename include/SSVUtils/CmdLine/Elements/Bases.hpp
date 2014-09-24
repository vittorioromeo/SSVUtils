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

				SSVU_DEFINE_SINK_SETTER_SIMPLE(setName, name)
				SSVU_DEFINE_SINK_SETTER_SIMPLE(setBriefDesc, briefDesc)
				SSVU_DEFINE_SINK_SETTER_SIMPLE(setDesc, desc)

				inline const auto& getName() const noexcept			{ return name; }
				inline const auto& getBriefDesc() const noexcept	{ return briefDesc; }
				inline const auto& getDesc() const noexcept			{ return desc; }

				inline virtual std::string getUsageStr() const		{ return ""s; }

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

		class ArgBase : public ElementBase
		{
			public:
				virtual void set(const std::string&) = 0;
				inline std::string getUsageStr() const override { return "(ARG " + getName() + ")"; }
		};

		class ArgPackBase : public ElementBase
		{
			protected:
				std::size_t min, max;

			public:
				inline ArgPackBase() noexcept : min{0}, max{0} { }
				inline ArgPackBase(std::size_t mMin, std::size_t mMax) noexcept : min{mMin}, max{mMax} { }

				virtual void set(const std::vector<std::string>&) = 0;

				inline auto isInfinite() const noexcept	{ return min == 0 && max == 0; }
				inline auto getMin() const noexcept		{ return min; }
				inline auto getMax() const noexcept		{ return max; }

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
				std::string nameShort, nameLong; // With prefix

			public:
				inline FlagBase(const std::string& mNameShort, const std::string& mNameLong) : nameShort{flagPrefixShort + mNameShort}, nameLong{flagPrefixLong + mNameLong} { }

				inline const auto& getNameShort() const noexcept				{ return nameShort; }
				inline const auto& getNameLong() const noexcept					{ return nameLong; }
				inline auto hasName(const std::string& mName) const noexcept	{ return mName == nameShort || mName == nameLong; }
		};

		class FlagValueBase : public FlagBase
		{
			public:
				inline FlagValueBase(const std::string& mNameShort, const std::string& mNameLong) : FlagBase{mNameShort, mNameLong} { }
				virtual void set(const std::string&) = 0;
		};
	}
}

#endif
