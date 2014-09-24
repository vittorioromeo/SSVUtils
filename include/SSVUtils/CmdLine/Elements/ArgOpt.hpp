// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_CMDLINE_ELEMENTS_ARGOPT
#define SSVU_CMDLINE_ELEMENTS_ARGOPT

namespace ssvu
{
	namespace CmdLine
	{
		template<typename T> class ArgOpt final : public Arg<T>
		{
			private:
				T defaultValue;
				bool active{false};

			public:
				inline ArgOpt(T mDefaultValue) noexcept : defaultValue{std::move(mDefaultValue)} { }

				inline operator bool() const noexcept				{ return active; }
				inline void set(const std::string& mValue) override { Arg<T>::set(mValue); active = true; }
				inline T get() const								{ return active ? Arg<T>::get() : defaultValue; }
				inline std::string getUsageStr() const override		{ return "(ARGOPT " + ElementBase::getName() + ")"; }
		};
	}
}

#endif
