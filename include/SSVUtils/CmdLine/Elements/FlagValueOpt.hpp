// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_CMDLINE_ELEMENTS_FLAGVALUEOPT
#define SSVU_CMDLINE_ELEMENTS_FLAGVALUEOPT

namespace ssvu
{
	namespace CmdLine
	{
		template<typename T> class FlagValueOpt final : public Internal::FlagValueImpl<T>, public Internal::ETypeInfo<EType::FlagValueOpt>
		{
			private:
				T valueDefault;
				bool active{false};

			public:
				inline FlagValueOpt(const std::string& mNameShort, const std::string& mNameLong, const T& mValueDefault) noexcept
					: Internal::FlagValueImpl<T>{mNameShort, mNameLong}, valueDefault{mValueDefault} { }

				inline void set(const std::string& mValue) override	{ FlagValue<T>::set(mValue); active = true; }
				inline auto get() const noexcept					{ return active ? FlagValue<T>::get() : valueDefault; }

				inline std::string getUsageStr() const override { return "[OPT " + Internal::BaseFlag::getNameShort() + "=(...) || OPT " + Internal::BaseFlag::getNameLong() + "=(...)]"; }
		};
	}
}

#endif
