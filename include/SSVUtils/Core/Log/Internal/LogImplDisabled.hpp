// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_CORE_LOG_INTERNAL_IMPL_DISABLED
#define SSVU_CORE_LOG_INTERNAL_IMPL_DISABLED

namespace ssvu
{
	namespace Internal
	{
		inline const std::string& getUniqueColor(const std::string&)
		{
			return Console::setColorFG(Console::Color::Default);
		}

		inline std::ostringstream& getLogStream() noexcept { static std::ostringstream logStream; return logStream; }

		struct LOut
		{
			inline void flush() const { }
		};

		template<typename T> inline LOut& operator<<(LOut& mLOut, const T&) { return mLOut; }
		inline LOut& operator<<(LOut& mLOut, StdEndLine) { return mLOut; }
		inline LOut& getLOutInstance() noexcept { static LOut loInstance; return loInstance; }

		inline Internal::LOut& lo() noexcept { return Internal::getLOutInstance(); }
		template<typename T> inline Internal::LOut& lo(const T&) { return lo(); }

		inline const char* hr() noexcept { return getEmptyString().c_str(); }
		inline std::string hr(int, char) { return getEmptyString(); }
	}
}

#endif
