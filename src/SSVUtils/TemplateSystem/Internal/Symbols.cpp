// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#include "SSVUtils/TemplateSystem/Internal/Symbols.h"

using namespace std;

namespace ssvu
{
	namespace TemplateSystem
	{
		namespace Internal
		{
			const string keyLeft{"{{"};
			const string keyRight{"}}"};
			const string sectionStartLeft{"{{#"};
			const string sectionStartRight{"}}"};
			const string sectionEndLeft{"{{/"};
			const string sectionEndRight{"}}"};

			string getKey(const std::string& mString) { return keyLeft + mString + keyRight; }
			string getSectionStart(const std::string& mString) { return sectionStartLeft + mString + sectionStartRight; }
			string getSectionEnd(const std::string& mString) { return sectionEndLeft + mString + sectionEndRight; }
		}
	}
}
