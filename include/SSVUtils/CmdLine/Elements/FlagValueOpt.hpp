// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_CMDLINE_ELEMENTS_FLAGVALUEOPT
#define SSVU_CMDLINE_ELEMENTS_FLAGVALUEOPT

namespace ssvu
{
	namespace CmdLine
	{
		template<typename T> class FlagValueOpt final : public FlagValue<T>
		{
			private:
				T defaultValue;
				bool active{false};

			public:
				inline FlagValueOpt(std::string mShortName, std::string mLongName, const T& mDefaultValue) noexcept : FlagValue<T>{std::move(mShortName), std::move(mLongName)}, defaultValue{mDefaultValue} { }

				inline void set(const std::string& mValue) override { FlagValue<T>::set(mValue); active = true; }
				inline T get() const noexcept						{ return active ? FlagValue<T>::get() : defaultValue; }

				inline std::string getUsageStr() const override { return "[OPT " + FlagBase::getShortNameWithPrefix() + "=(...) || OPT " + FlagBase::getLongNameWithPrefix() + "=(...)]"; }
		};
	}
}

#endif
