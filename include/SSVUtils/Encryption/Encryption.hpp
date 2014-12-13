// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_ENCRYPTION
#define SSVU_ENCRYPTION

#include "SSVUtils/Internal/API.hpp"
#include "SSVUtils/Encryption/Inc/Internal/Base64.hpp"
#include "SSVUtils/Encryption/Inc/Internal/MD5.hpp"
#include "SSVUtils/Encryption/Inc/Encryption.hpp"
#include "SSVUtils/Encryption/Inc/ObfuscatedValue.hpp"

#if defined(SSVU_API_HEADERONLY)
	#include "SSVUtils/Encryption/Src/Base64.cpp"
	#include "SSVUtils/Encryption/Src/MD5.cpp"
	#include "SSVUtils/Encryption/Src/Encryption.cpp"
#endif

#endif
