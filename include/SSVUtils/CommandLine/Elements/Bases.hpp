// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_COMMANDLINE_ELEMENTS_BASES
#define SSVU_COMMANDLINE_ELEMENTS_BASES

namespace ssvu
{
	namespace CommandLine
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

				inline const std::string& getName() const noexcept		{ return name; }
				inline const std::string& getBriefDesc() const noexcept	{ return briefDesc; }
				inline const std::string& getDesc() const noexcept		{ return desc; }
				inline virtual std::string getUsageStr() const			{ return ""; }
				inline std::string getHelpStr()
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

				inline bool isInfinite() const noexcept		{ return min == 0 && max == 0; }
				inline std::size_t getMin() const noexcept	{ return min; }
				inline std::size_t getMax() const noexcept	{ return max; }
				inline std::string getUsageStr() const override
				{
					return "(PACK " + getName() + " " + "[" + toStr(min) + "/" + (isInfinite() ? "..." : toStr(max)) + "])";
				}
		};
	}
}

#endif
