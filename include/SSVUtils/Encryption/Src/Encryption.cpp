// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#include "SSVUtils/Internal/API.hpp"
#include "SSVUtils/Core/Core.hpp"
#include "SSVUtils/Encryption/Inc/Encryption.hpp"
#include "SSVUtils/Encryption/Inc/Internal/Base64.hpp"
#include "SSVUtils/Encryption/Inc/Internal/MD5.hpp"

namespace ssvu
{
	namespace Encryption
	{
		template<> SSVU_INLINE std::string encrypt<Type::Base64>(const std::string& mStr)
		{
			return Internal::Base64Encode(mStr);
		}
		template<> SSVU_INLINE std::string encrypt<Type::MD5>(const std::string& mStr)
		{
			return Internal::MD5{mStr}.GetHash();
		}

		template<> SSVU_INLINE std::string decrypt<Type::Base64>(const std::string& mStr)
		{
			return Internal::Base64Decode(mStr);
		}
	}
}
