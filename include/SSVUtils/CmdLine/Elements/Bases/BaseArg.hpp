// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_CMDLINE_ELEMENTS_BASES_BASEARG
#define SSVU_CMDLINE_ELEMENTS_BASES_BASEARG

namespace ssvu
{
	namespace CmdLine
	{
		namespace Internal
		{
			struct BaseArg : public BaseElement
			{
				virtual void set(const std::string&) = 0;
				inline std::string getUsageStr() const override { return "(ARG " + getName() + ")"; }
			};
		}
	}
}

#endif
