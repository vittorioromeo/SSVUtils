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
		inline const std::string& getUniqueColor(const std::string& mStr)
		{
			static int lastColorIdx{2};
			static std::unordered_map<std::string, Console::Color> map;

			if(map.count(mStr) == 0) map[mStr] = Console::Color(getWrapIdx(lastColorIdx++, 2, 7));
			return Console::setColorFG(map[mStr]);
		}
	}

	/// @brief Returns a reference to the log stream.
	inline std::ostringstream& getLogStream() noexcept { static std::ostringstream logStream; return logStream; }

	namespace Internal
	{
		using CoutType = std::basic_ostream<char, std::char_traits<char>>;
		using StdEndLine = CoutType&(CoutType&);

		struct LOut
		{
			std::string title;
			inline void flush() const { std::cout.flush(); getLogStream().flush(); }
		};

		template<typename T> inline LOut& operator<<(LOut& mLOut, const T& mValue)
		{
			#ifndef SSVU_LOG_DISABLE
				if(mLOut.title != "")
				{
					const auto& tStr("[" + mLOut.title + "] ");
					std::cout << Internal::getUniqueColor(mLOut.title) << Console::setStyle(Console::Style::Bold) << std::left << std::setw(38) << tStr;
					getLogStream() << std::left << std::setw(38) << tStr;
					mLOut.title = "";
				}

				std::cout << Console::resetFmt();
				stringify<true>(std::cout, mValue);
				std::cout << Console::resetFmt();

				stringify<false>(getLogStream(), mValue);
			#endif

			return mLOut;
		}
		inline LOut& operator<<(LOut& mLOut, StdEndLine manip)
		{
			#ifndef SSVU_LOG_DISABLE
				manip(std::cout); manip(getLogStream());
			#endif

			return mLOut;
		}

		inline LOut& getLOutInstance() noexcept { static LOut loInstance; return loInstance; }
	}

	/// @brief Returns a reference to the "log stream" singleton. (no title)
	inline Internal::LOut& lo() noexcept { return Internal::getLOutInstance(); }

	/// @brief Returns a reference to the "log stream" singleton. (sets title)
	/// @param mTitle Title of the next log message.
	template<typename T> inline Internal::LOut& lo(const T& mTitle)
	{
		lo().title = toStr(mTitle);
		return lo();
	}

}

#endif
