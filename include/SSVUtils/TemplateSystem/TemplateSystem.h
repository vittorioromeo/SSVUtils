// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_TEMPLATESYSTEM
#define SSVU_TEMPLATESYSTEM

#include <string>
#include <vector>
#include <map>
#include "SSVUtils/Utils/UtilsContainers.h"
#include "SSVUtils/Utils/UtilsString.h"

namespace ssvu
{
	namespace TemplateSystem
	{

		const std::string keySymbolLeft{"{{"};
		const std::string keySymbolRight{"}}"};
		const std::string sectionSymbolStartLeft{"{{#"};
		const std::string sectionSymbolStartRight{"}}"};
		const std::string sectionSymbolEndLeft{"{{/"};
		const std::string sectionSymbolEndRight{"}}"};

		class SectionReplacement
		{
			private:

			public:
		};

		class Dictionary
		{
			private:
				std::map<std::string, std::string> stringReplacements;
				std::map<std::string, std::vector<Dictionary>> sectionReplacements;

			public:
				std::string getExpanded(const std::string& mString);

				void setStringReplacement(const std::string& mKey, const std::string& mValue);
				void addSectionReplacement(const std::string& mKey, const Dictionary& mValue);
		};
	}
}

#endif
