// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVUTILS_COMMANDLINE_ELEMENTS_BASES_ARGBASE
#define SSVUTILS_COMMANDLINE_ELEMENTS_BASES_ARGBASE

#include <string>
#include "SSVUtils/CommandLine/Elements/Bases/ElementBase.h"

namespace ssvu
{
	namespace CommandLine
	{
		class ArgBase : public ElementBase
		{
			public:
				virtual ~ArgBase() { }
				virtual void set(const std::string& mString) = 0;
				inline std::string getUsageStr() const override { return "(ARG " + getName() + ")"; }
		};
	}
}

#endif
