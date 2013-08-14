// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVUTILS_COMMANDLINE_ELEMENTS_BASES_ARGPACKBASE
#define SSVUTILS_COMMANDLINE_ELEMENTS_BASES_ARGPACKBASE

#include <string>
#include <vector>
#include "SSVUtils/String/Utils.h"
#include "SSVUtils/CommandLine/Elements/Bases/ElementBase.h"

namespace ssvu
{
	namespace CommandLine
	{
		class ArgPackBase : public ElementBase
		{
			protected:
				bool infinite;
				unsigned int min, max;

			public:
				ArgPackBase() : infinite{true} { }
				ArgPackBase(unsigned int mMin, unsigned int mMax) : infinite{false}, min{mMin}, max{mMax} { }

				virtual void set(const std::vector<std::string>& mStrings) = 0;

				inline bool isInfinite() const		{ return infinite; }
				inline unsigned int getMin() const	{ return min; }
				inline unsigned int getMax() const	{ return max; }
				inline std::string getUsageStr() const override
				{
					return "(PACK " + getName() + " " + "[" + toStr(min) + "/" + (infinite ? "..." : toStr(max)) + "])";
				}
		};
	}
}

#endif
