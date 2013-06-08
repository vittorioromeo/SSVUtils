// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_ENCRYPTION_INTERNAL_BASE64
#define SSVU_ENCRYPTION_INTERNAL_BASE64

#include <string>

namespace ssvu
{
	namespace Encryption
	{
		namespace Internal
		{
			std::string Base64Encode(const std::string& mString);
			std::string Base64Decode(const std::string& mString);
		}
	}
}

#endif
