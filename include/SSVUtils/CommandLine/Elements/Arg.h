// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVUTILS_COMMANDLINE_ELEMENTS_ARG
#define SSVUTILS_COMMANDLINE_ELEMENTS_ARG

#include <string>
#include "SSVUtils/CommandLine/Parser.h"

namespace ssvu
{
	namespace CommandLine
	{
		template<typename T> class Arg : public ArgBase
		{
			protected:
				T value;

			public:
				inline T get() const { return value; }
				inline void set(const std::string& mValue) override { value = Parser<T>::parse(mValue); }
		};
	}
}

#endif
