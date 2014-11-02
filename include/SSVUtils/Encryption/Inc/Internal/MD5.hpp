// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_IMPL_ENCRYPTION_INTERNAL_MD5
#define SSVU_IMPL_ENCRYPTION_INTERNAL_MD5

/* Copyright (C) 1991-2, RSA Data Security, Inc. Created 1991. All
rights reserved.

License to copy and use this software is granted provided that it
is identified as the "RSA Data Security, Inc. MD5 Message-Digest
Algorithm" in all material mentioning or referencing this software
or this function.

License is also granted to make and use derivative works provided
that such works are identified as "derived from the RSA Data
Security, Inc. MD5 Message-Digest Algorithm" in all material
mentioning or referencing the derived work.

RSA Data Security, Inc. makes no representations concerning either
the merchantability of this software or the suitability of this
software for any particular purpose. It is provided "as is"
without express or implied warranty of any kind.

These notices must be retained in any copies of any part of this
documentation and/or software. */

namespace ssvu
{
	namespace Encryption
	{
		namespace Internal
		{
			class MD5
			{
				private:
					std::string m_sHash;
					unsigned char m_rawHash[16];

				public:
					MD5() = default;
					MD5(const std::string& source) { Calculate(source); }
					MD5(std::ifstream& file) { Calculate(file); }
					MD5(const unsigned char* source, unsigned int len) { Calculate(source, len); }

					std::string Calculate(const std::string& source) { return Calculate((const unsigned char*)source.c_str(), source.size()); }
					std::string Calculate(std::ifstream& file);
					std::string Calculate(const unsigned char* source, uint32_t len);

					std::string GetHash() const { return m_sHash; }
					const unsigned char* GetRawHash() const { return m_rawHash; }
			};
		}
	}
}

#endif
