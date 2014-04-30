// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_CORE_LOG
#define SSVU_CORE_LOG

#include <string>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <ostream>
#include <fstream>
#include <chrono>
#include <unordered_map>
#include "SSVUtils/Core/String/String.hpp"
#include "SSVUtils/Core/ConsoleFmt/ConsoleFmt.hpp"
#include "SSVUtils/Core/Utils/Math.hpp"

namespace ssvu
{
	namespace FileSystem { class Path; }

	namespace Internal
	{
		using CoutType = std::basic_ostream<char, std::char_traits<char>>;
		using StdEndLine = CoutType&(CoutType&);
	}
}

#ifndef SSVU_LOG_DISABLE
	#include "SSVUtils/Core/Log/Internal/LogImplEnabled.hpp"
#else
	#include "SSVUtils/Core/Log/Internal/LogImplDisabled.hpp"
#endif

#endif
