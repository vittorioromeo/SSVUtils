
#include "SSVUtils/Encryption/Encryption.h"
#include "SSVUtils/Encryption/Internal/Base64.h"
#include "SSVUtils/Encryption/Internal/MD5.h"

using namespace std;

namespace ssvu
{
	namespace Encryption
	{
		template<> string encrypt<Type::BASE64>(const string& mString) { return Base64Encode(mString); }
		template<> string encrypt<Type::MD5>(const string& mString) { return MD5{mString}.GetHash(); }
		template<> string decrypt<Type::BASE64>(const string& mString) { return Base64Decode(mString); }
	}
}

