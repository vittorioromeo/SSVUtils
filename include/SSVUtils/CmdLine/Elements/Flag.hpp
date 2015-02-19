// Copyright (c) 2013-2015 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_CMDLINE_ELEMENTS_FLAG
#define SSVU_CMDLINE_ELEMENTS_FLAG

namespace ssvu
{
	namespace CmdLine
	{
		class Flag final : public Impl::BaseFlag, public Impl::ETypeInfo<EType::Flag>
		{
			private:
				bool active{false};

			public:
				inline Flag(const std::string& mNameShort, const std::string& mNameLong) noexcept : Impl::BaseFlag{mNameShort, mNameLong} { }

				inline auto& operator=(bool mActive) noexcept	{ active = mActive; return *this; }
				inline operator bool() const noexcept			{ return active; }

				inline std::string getUsageStr() const override { return "[" + getNameShort() + " || " + getNameLong() + "]"; }
		};
	}
}

#endif
