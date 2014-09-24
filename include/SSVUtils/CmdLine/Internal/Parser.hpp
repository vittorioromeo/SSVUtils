// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_CMDLINE_INTERAL_PARSER
#define SSVU_CMDLINE_INTERAL_PARSER

namespace ssvu
{
	namespace CmdLine
	{
		namespace Internal
		{
			template<typename T> struct Parser;

			template<> struct Parser<std::string>
			{
				inline static auto parse(const std::string& mStr) { return mStr; }
			};

			template<> struct Parser<int>
			{
				inline static int parse(const std::string& mStr)
				{
					try { return std::stoi(mStr); }
					catch(...) { throw std::runtime_error("Cannot parse '" + mStr + "' to int"); }
				}
			};

			template<> struct Parser<float>
			{
				inline static float parse(const std::string& mStr)
				{
					try { return std::stof(mStr); }
					catch(...) { throw std::runtime_error("Cannot parse '" + mStr + "' to float"); }
				}
			};

			template<> struct Parser<bool>
			{
				inline static bool parse(const std::string& mStr)
				{
					static std::string trueValues[]{"y", "yes", "on", "true", "1"};
					static std::string falseValues[]{"n", "no", "off", "false", "0"};
					if(contains(trueValues, toLower(mStr))) return true;
					if(contains(falseValues, toLower(mStr))) return false;
					throw std::runtime_error("Cannot parse '" + mStr + "' to bool");
				}
			};
		}
	}
}

#endif
