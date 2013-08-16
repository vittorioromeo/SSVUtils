// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_COMMANDLINE_PARSER
#define SSVU_COMMANDLINE_PARSER

#include <vector>
#include <string>
#include <stdexcept>
#include "SSVUtils/Utils/UtilsContainers.h"
#include "SSVUtils/String/Utils.h"

namespace ssvu
{
	namespace CommandLine
	{
		template<typename T> struct Parser;
		template<> struct Parser<int>
		{
			inline static int parse(const std::string& mString)
			{
				try { return std::stoi(mString); }
				catch(...) { throw std::runtime_error("Cannot parse '" + mString + "' to int"); }
			}
		};
		template<> struct Parser<std::string>
		{
			inline static std::string parse(const std::string& mString) { return mString; }
		};
		template<> struct Parser<bool>
		{
			inline static bool parse(const std::string& mString)
			{
				static std::vector<std::string> trueValues{"y", "yes", "on", "true", "1"};
				static std::vector<std::string> falseValues{"n", "no", "off", "false", "0"};
				if(contains(trueValues, toLower(mString))) return true;
				if(contains(falseValues, toLower(mString))) return false;
				throw std::runtime_error("Cannot parse '" + mString + "' to bool");
			}
		};
	}
}

#endif
