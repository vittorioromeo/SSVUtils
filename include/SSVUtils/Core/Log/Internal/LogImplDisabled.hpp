// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_CORE_LOG_INTERNAL_IMPL_DISABLED
#define SSVU_CORE_LOG_INTERNAL_IMPL_DISABLED

namespace ssvu
{
	namespace Internal
	{
		inline const auto& getUniqueColor(const std::string&)
		{
			return Console::setColorFG(Console::Color::Default);
		}

		struct LOut
		{
			inline void flush() const { }
		};

		template<typename T> inline LOut& operator<<(LOut& mLOut, const T&) { return mLOut; }
		inline LOut& operator<<(LOut& mLOut, StdEndLine) { return mLOut; }

		inline LOut& lo() noexcept { static LOut result; return result; }
		template<typename T> inline LOut& lo(const T&) { return lo(); }

		inline const char* hr() noexcept { return getEmptyString().c_str(); }
		inline std::string hr(int, char) { return getEmptyString(); }
	}
}

#endif
