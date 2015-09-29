// Copyright (c) 2013-2015 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_CMDLINE_INTERAL_PARSER
#define SSVU_CMDLINE_INTERAL_PARSER

#include "SSVUtils/Core/Core.hpp"

namespace ssvu
{
    namespace CmdLine
    {
        namespace Impl
        {
            template <typename T>
            struct Parser;

            template <>
            struct Parser<std::string>
            {
                inline static auto parse(const std::string& mStr)
                {
                    return mStr;
                }
            };

            template <>
            struct Parser<int>
            {
                inline static int parse(const std::string& mStr)
                {
                    try
                    {
                        return sToInt(mStr);
                    }
                    catch(...)
                    {
                        throw std::runtime_error("Cannot parse '" + mStr +
                                                 "' to int");
                    }
                }
            };

            template <>
            struct Parser<float>
            {
                inline static float parse(const std::string& mStr)
                {
                    try
                    {
                        return sToFloat(mStr);
                    }
                    catch(...)
                    {
                        throw std::runtime_error("Cannot parse '" + mStr +
                                                 "' to float");
                    }
                }
            };

            template <>
            struct Parser<bool>
            {
                inline static bool parse(const std::string& mStr)
                {
                    static std::string trueValues[]{"y", "yes", "on", "true",
                                                    "1"};
                    static std::string falseValues[]{"n", "no", "off", "false",
                                                     "0"};
                    if(contains(trueValues, toLower(mStr))) return true;
                    if(contains(falseValues, toLower(mStr))) return false;
                    throw std::runtime_error("Cannot parse '" + mStr +
                                             "' to bool");
                }
            };
        }
    }
}

#endif
