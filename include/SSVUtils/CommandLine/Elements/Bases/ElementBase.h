// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVUTILS_COMMANDLINE_ELEMENTS_BASES_ELEMENTBASE
#define SSVUTILS_COMMANDLINE_ELEMENTS_BASES_ELEMENTBASE

#include <string>

namespace ssvu
{
	namespace CommandLine
	{
		class ElementBase
		{
			private:
				std::string name, briefDesc, desc;

			public:
				virtual ~ElementBase() { }

				inline void setName(const std::string& mName)			{ name = mName; }
				inline void setBriefDesc(const std::string& mBriefDesc)	{ briefDesc = mBriefDesc; }
				inline void setDesc(const std::string& mDesc)			{ desc = mDesc; }

				inline const std::string& getName() const		{ return name; }
				inline const std::string& getBriefDesc() const	{ return briefDesc; }
				inline const std::string& getDesc() const		{ return desc; }
				inline virtual std::string getUsageStr() const	{ return ""; }
				inline std::string getHelpStr()
				{
					std::string result, usageStr{this->getUsageStr()};

					if(!usageStr.empty()) result += "* " + usageStr;
					if(!name.empty()) result += "\n  --" + name;
					if(!briefDesc.empty()) result += "\n  --" + briefDesc;
					if(!desc.empty()) result += "\n  --" + desc;

					return result + "\n\n";
				}
		};
	}
}

#endif
