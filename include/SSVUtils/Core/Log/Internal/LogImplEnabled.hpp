// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_CORE_LOG_INTERNAL_IMPL_ENABLED
#define SSVU_CORE_LOG_INTERNAL_IMPL_ENABLED

namespace ssvu
{
	namespace Internal
	{
		inline const std::string& getUniqueColor(const std::string& mStr)
		{
			static int lastColorIdx{2};
			static std::unordered_map<std::string, Console::Color> map;

			if(map.count(mStr) == 0) map[mStr] = Console::Color(getMod(lastColorIdx++, 2, 7));
			return Console::setColorFG(map[mStr]);
		}

		inline std::ostringstream& getLogStream() noexcept { static std::ostringstream logStream; return logStream; }

		struct LOut
		{
			std::string title;
			inline void flush() const { std::cout.flush(); getLogStream().flush(); }
		};

		template<typename T> inline LOut& operator<<(LOut& mLOut, const T& mValue)
		{
			constexpr std::size_t leftW{38};

			if(mLOut.title != "")
			{
				const auto& tStr("[" + mLOut.title + "] ");
				std::cout << getUniqueColor(mLOut.title) << Console::setStyle(Console::Style::Bold) << std::left << std::setw(leftW) << tStr;
				getLogStream() << std::left << std::setw(leftW) << tStr;
				mLOut.title = "";
			}

			std::cout << Console::resetFmt();
			stringify<true>(std::cout, mValue);
			std::cout << Console::resetFmt();

			stringify<false>(getLogStream(), mValue);

			return mLOut;
		}
		inline LOut& operator<<(LOut& mLOut, StdEndLine mManip)
		{
			mManip(std::cout); mManip(getLogStream());
			return mLOut;
		}

		inline LOut& getLOutInstance() noexcept { static LOut loInstance; return loInstance; }

		inline LOut& lo() noexcept { return getLOutInstance(); }

		template<typename T> inline LOut& lo(const T& mTitle)
		{
			lo().title = toStr(mTitle);
			return lo();
		}

		inline const char* hr() noexcept				{ static std::string str(Console::Info::getColumnCount(), '_'); return str.c_str(); }
		inline std::string hr(int mOffset, char mChar)	{ return std::string(Console::Info::getColumnCount() + mOffset, mChar); }
	}
}

#endif

// TODO: investigate non-working manipulators (example: setw)
// investigate possibility of counting characters printed in a certain line in order to create automatic-length hr()
