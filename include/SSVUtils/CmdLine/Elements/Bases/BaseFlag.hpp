// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_CMDLINE_ELEMENTS_BASES_BASEFLAG
#define SSVU_CMDLINE_ELEMENTS_BASES_BASEFLAG

namespace ssvu
{
	namespace CmdLine
	{
		namespace Internal
		{
			constexpr const char* flagPrefixShort{"-"};
			constexpr const char* flagPrefixLong{"--"};

			class BaseFlag : public BaseElement
			{
				private:
					std::string nameShort, nameLong; // With prefix

				public:
					inline BaseFlag(const std::string& mNameShort, const std::string& mNameLong) : nameShort{flagPrefixShort + mNameShort}, nameLong{flagPrefixLong + mNameLong} { }

					inline const auto& getNameShort() const noexcept				{ return nameShort; }
					inline const auto& getNameLong() const noexcept					{ return nameLong; }
					inline auto hasName(const std::string& mName) const noexcept	{ return mName == nameShort || mName == nameLong; }
			};
		}
	}
}

#endif
