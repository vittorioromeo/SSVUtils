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
			std::string getKey(const std::string& mString);
			std::string getSectionStart(const std::string& mString);
			std::string getSectionEnd(const std::string& mString);
		}
	}
}

#endif
