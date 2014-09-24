// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_CMDLINE_ELEMENTS_ARG
#define SSVU_CMDLINE_ELEMENTS_ARG

namespace ssvu
{
	namespace CmdLine
	{
		template<typename T> class Arg : public ArgBase
		{
			protected:
				T value;

			public:
				inline T get() const noexcept { return value; }
				inline void set(const std::string& mValue) override { value = Parser<T>::parse(mValue); }
		};
	}
}

#endif
