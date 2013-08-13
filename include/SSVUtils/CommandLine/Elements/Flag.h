// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVUTILS_COMMANDLINE_ELEMENTS_FLAG
#define SSVUTILS_COMMANDLINE_ELEMENTS_FLAG

#include <string>
#include "SSVUtils/CommandLine/Elements/Bases/ElementBase.h"

namespace ssvu
{
	namespace CommandLine
	{
		const std::string flagPrefixShort{"-"};
		const std::string flagPrefixLong{"--"};

		class Flag : public ElementBase
		{
			private:
				std::string shortName, longName;
				bool active{false};

			public:
				Flag(const std::string& mShortName, const std::string& mLongName) : shortName{mShortName}, longName{mLongName} { }

				inline Flag& operator=(bool mActive)	{ active = mActive; return *this; }
				inline operator bool() const			{ return active; }

				inline const std::string& getShortName() const		{ return shortName; }
				inline const std::string& getLongName() const		{ return longName; }
				inline std::string getShortNameWithPrefix() const	{ return flagPrefixShort + shortName; }
				inline std::string getLongNameWithPrefix() const	{ return flagPrefixLong + longName; }
				inline bool hasName(const std::string& mName) const	{ return mName == getShortNameWithPrefix() || mName == getLongNameWithPrefix(); }
				inline std::string getUsageStr() const override
				{
					return "[" + getShortNameWithPrefix() + " || " + getLongNameWithPrefix() + "]";
				}
		};
	}
}

#endif
