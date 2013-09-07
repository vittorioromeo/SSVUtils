// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_COMMANDLINE_ELEMENTS_OPTARG
#define SSVU_COMMANDLINE_ELEMENTS_OPTARG

#include <string>
#include "SSVUtils/CommandLine/Parser.h"

namespace ssvu
{
	namespace CommandLine
	{
		template<typename T> class OptArg : public Arg<T>
		{
			private:
				T defaultValue;
				bool active{false};

			public:
				OptArg(T mDefaultValue) noexcept : defaultValue{std::move(mDefaultValue)} { }

				inline operator bool() const noexcept				{ return active; }
				inline void set(const std::string& mValue) override { Arg<T>::set(mValue); active = true; }
				inline T get() const								{ return active ? Arg<T>::get() : defaultValue; }
				inline std::string getUsageStr() const override		{ return "(OPTARG " + ElementBase::getName() + ")"; }
		};
	}
}

#endif
