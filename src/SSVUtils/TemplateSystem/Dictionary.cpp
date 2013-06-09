// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#include "SSVUtils/TemplateSystem/Dictionary.h"
#include "SSVUtils/TemplateSystem/Internal/Symbols.h"
#include "SSVUtils/Utils/UtilsString.h"

using namespace std;
using namespace ssvu::TemplateSystem::Internal;

namespace ssvu
{
	namespace TemplateSystem
	{
		Dictionary::Dictionary(initializer_list<pair<string, string>> mPairs) { for(const auto& p : mPairs) replacements.insert(p); }

		string Dictionary::getExpanded(const string& mString) const
		{
			string result{mString};

			for(const auto& p : sectionDictionaries)
			{
				const string sectionStart{getSectionStart(p.first)}, sectionEnd{getSectionEnd(p.first)};
				const auto outerSectionStartIndex(result.find(sectionStart));
				const auto innerSectionEndIndex(result.find(sectionEnd));
				const auto innerSectionStartIndex(outerSectionStartIndex + sectionStart.length());
				const auto outerSectionEndIndex(innerSectionEndIndex + sectionEnd.length());

				const string innerSection{result.substr(innerSectionStartIndex, innerSectionEndIndex - innerSectionStartIndex)};
				string innerSectionResult{""};

				for(auto d : p.second) innerSectionResult += d.getExpanded(innerSection);

				result.replace(outerSectionStartIndex, outerSectionEndIndex - outerSectionStartIndex, innerSectionResult);
			}

			for(const auto& p : replacements) replaceAll(result, getKey(p.first), p.second);

			return result;
		}

		string& Dictionary::operator[](const string& mKey) { return replacements[mKey]; }
		void Dictionary::operator+=(const pair<string, Dictionary>& mPair) { sectionDictionaries[mPair.first].push_back(mPair.second); }
	}
}
