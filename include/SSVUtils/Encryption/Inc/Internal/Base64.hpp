// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_IMPL_ENCRYPTION_INTERNAL_BASE64
#define SSVU_IMPL_ENCRYPTION_INTERNAL_BASE64

namespace ssvu
{
	namespace Encryption
	{
		namespace Internal
		{
			std::string Base64Encode(const std::string& mStr);
			std::string Base64Decode(const std::string& mStr);
		}
	}
}

#endif
