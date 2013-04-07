// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_ENCRYPTION_BASE64
#define SSVU_ENCRYPTION_BASE64

#include <string>

namespace ssvu
{
	std::string base64_encode(const std::string& mString);
	std::string base64_encode(unsigned char const* , unsigned int len);
	std::string base64_decode(std::string const& s);
}

#endif

// TODO: put all encryption stuff in an Encryption namespace and create consistent public interfaces
