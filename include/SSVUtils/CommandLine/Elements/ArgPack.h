// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVUTILS_COMMANDLINE_ELEMENTS_ARGPACK
#define SSVUTILS_COMMANDLINE_ELEMENTS_ARGPACK

#include <string>
#include <vector>
#include "SSVUtils/CommandLine/Parser.h"

namespace ssvu
{
	namespace CommandLine
	{
		template<typename T> class ArgPack : public ArgPackBase
		{
			private:
				using Iterator = typename std::vector<T>::iterator;
				using ConstIterator = typename std::vector<T>::const_iterator;

				std::vector<T> values;

			public:
				ArgPack() = default;
				ArgPack(unsigned int mMin, unsigned int mMax) : ArgPackBase{mMin, mMax} { }

				inline void set(const std::vector<std::string>& mValues) override
				{
					for(const auto& v : mValues) values.push_back(Parser<T>::parse(v));
				}

				inline Iterator begin()					{ return values.begin(); }
				inline Iterator end()					{ return values.end(); }
				inline ConstIterator begin() const		{ return values.begin(); }
				inline ConstIterator end() const		{ return values.end(); }
				inline const Iterator cbegin() const	{ return values.cbegin(); }
				inline const Iterator cend() const		{ return values.cend(); }
		};
	}
}

#endif
