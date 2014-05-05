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

namespace ssvu
{
	/// @brief Returns a reference to the log stream.
	inline std::ostringstream& getLogStream() noexcept { return Internal::getLogStream(); }

	/// @brief Returns a reference to the "log stream" singleton. (no title)
	inline Internal::LOut& lo() noexcept { return Internal::getLOutInstance(); }

	/// @brief Returns a reference to the "log stream" singleton. (sets title)
	/// @param mTitle Title of the next log message.
	template<typename T> inline Internal::LOut& lo(const T& mTitle) { return Internal::lo(mTitle); }

	/// @brief Returns a const char* horizontal ruler C string. Intended to be used when printing to the console.
	inline const char* hr() noexcept { return Internal::hr(); }

	/// @brief Returns a std::string customized horizontal ruler. Intended to be used when printing to the console.
	/// @param mOffset Length offset of the ruler.
	/// @param mChar Character used to create the ruler.
	inline std::string hr(int mOffset, char mChar = '_') { return Internal::hr(mOffset, mChar); }
}

#endif
