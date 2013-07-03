// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#include "SSVUtils/Encryption/Encryption.h"
#include "SSVUtils/Encryption/Internal/Base64.h"
#include "SSVUtils/Encryption/Internal/MD5.h"

using namespace std;

namespace ssvu
{
	namespace Encryption
	{
		template<> string encrypt<Type::Base64>(const string& mString)	{ return Internal::Base64Encode(mString); }
		template<> string encrypt<Type::MD5>(const string& mString)		{ return Internal::MD5{mString}.GetHash(); }
		template<> string decrypt<Type::Base64>(const string& mString)	{ return Internal::Base64Decode(mString); }
	}
}

