// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_CMDLINE_INTERNAL_ETYPEBASEINFO
#define SSVU_CMDLINE_INTERNAL_ETYPEBASEINFO

namespace ssvu
{
	namespace CmdLine
	{
		namespace Internal
		{
			template<EType TET> struct ETypeBaseInfo;

			template<> struct ETypeBaseInfo<EType::Arg>				{ using Type = BaseArg; };
			template<> struct ETypeBaseInfo<EType::ArgOpt>			{ using Type = BaseArg; };
			template<> struct ETypeBaseInfo<EType::ArgPack>			{ using Type = BaseArgPack; };
			template<> struct ETypeBaseInfo<EType::Flag>			{ using Type = BaseFlag; };
			template<> struct ETypeBaseInfo<EType::FlagValue>		{ using Type = BaseFlagValue; };
			template<> struct ETypeBaseInfo<EType::FlagValueOpt>	{ using Type = BaseFlagValue; };

			template<EType TET> using ETypeBase = typename ETypeBaseInfo<TET>::Type;
		}
	}
}

#endif
