// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_TEMPLATESYSTEM_INTERNAL_SYMBOLS
#define SSVU_TEMPLATESYSTEM_INTERNAL_SYMBOLS

#include <string>

namespace ssvu
{
	namespace TemplateSystem
	{
		namespace Internal
		{
			const static std::string keyLeft{"{{"};
			const static std::string keyRight{"}}"};
			const static std::string sectionStartLeft{"{{#"};
			const static std::string sectionStartRight{"}}"};
			const static std::string sectionEndLeft{"{{/"};
			const static std::string sectionEndRight{"}}"};

			inline static std::string getKey(const std::string& mString)			{ return keyLeft + mString + keyRight; }
			inline static std::string getSectionStart(const std::string& mString)	{ return sectionStartLeft + mString + sectionStartRight; }
			inline static std::string getSectionEnd(const std::string& mString)		{ return sectionEndLeft + mString + sectionEndRight; }
		}
	}
}

#endif
