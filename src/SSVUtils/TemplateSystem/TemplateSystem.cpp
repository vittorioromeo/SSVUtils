// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#include "SSVUtils/TemplateSystem/TemplateSystem.h"

using namespace std;

namespace ssvu
{
	namespace TemplateSystem
	{
		string Dictionary::getExpanded(const string& mString)
		{
			string result{mString};

			for(auto& p : sectionReplacements)
			{
				auto outerSectionStartIndex(result.find(sectionSymbolStartLeft + p.first + sectionSymbolStartRight));
				auto innerSectionEndIndex(result.find(sectionSymbolEndLeft + p.first + sectionSymbolEndRight));
				auto innerSectionStartIndex(outerSectionStartIndex + sectionSymbolStartLeft.length() + p.first.length() + sectionSymbolStartRight.length());
				auto outerSectionEndIndex(innerSectionEndIndex + sectionSymbolEndLeft.length() + p.first.length() + sectionSymbolEndRight.length());

				string innerSection{result.substr(innerSectionStartIndex, innerSectionEndIndex - innerSectionStartIndex)};
				string innerSectionResult{""};

				for(auto d : p.second) innerSectionResult += d.getExpanded(innerSection);

				result.replace(outerSectionStartIndex, outerSectionEndIndex - outerSectionStartIndex, innerSectionResult);
			}

			for(auto& p : stringReplacements) replaceAll(result, keySymbolLeft + p.first + keySymbolRight, p.second);

			return result;
		}

		void Dictionary::setStringReplacement(const string& mKey, const string& mValue)
		{
			stringReplacements[mKey] = mValue;
		}
		void Dictionary::addSectionReplacement(const string& mKey, const Dictionary& mValue)
		{
			sectionReplacements[mKey].push_back(mValue);
		}
	}
}
