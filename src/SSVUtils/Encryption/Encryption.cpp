
#include "SSVUtils/Encryption/Encryption.h"
#include "SSVUtils/Encryption/Internal/Base64.h"
#include "SSVUtils/Encryption/Internal/MD5.h"

using namespace std;

namespace ssvu
{
	namespace Encryption
	{
		template<> string encrypt<Type::Base64>(const string& mString) { return Internal::Base64Encode(mString); }
		template<> string encrypt<Type::MD5>(const string& mString) { return Internal::MD5{mString}.GetHash(); }
		template<> string decrypt<Type::Base64>(const string& mString) { return Internal::Base64Decode(mString); }
	}
}

