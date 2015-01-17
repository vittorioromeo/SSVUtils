// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_CORE_LOG
#define SSVU_CORE_LOG

namespace ssvu
{
	namespace FileSystem { class Path; }

	namespace Internal
	{
		using CoutType = decltype(std::cout);
		using StdEndLine = CoutType&(CoutType&);

		/// @brief Returns a reference to the static `std::ostringstream` log stream.
		inline auto& getLogStream() noexcept { static std::ostringstream logStream; return logStream; }

		/// @brief Returns a reference to the static suppressed `bool` value.
		inline auto& getLogSuppressed() noexcept { static bool logSuppressed{false}; return logSuppressed; }

		/// @brief Log object that does nothing but can be used in place of `LOut`.
		struct NullLOut
		{
			inline void flush() const { }
		};

		template<typename T> inline NullLOut& operator<<(NullLOut& mLOut, const T&) { return mLOut; }
		inline NullLOut& operator<<(NullLOut& mLOut, StdEndLine) { return mLOut; }
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
	inline auto& getLogStream() noexcept { return Internal::getLogStream(); }

	/// @brief Returns a reference to the "log stream" singleton. (no title)
	inline auto& lo() noexcept { return Internal::lo(); }

	/// @brief Returns a reference to the "log stream" singleton. (sets title)
	/// @param mTitle Title of the next log message.
	template<typename T> inline auto& lo(const T& mTitle) { return Internal::lo(mTitle); }

	/// @brief Returns a const char* horizontal ruler C string. Intended to be used when printing to the console.
	inline const char* hr() noexcept { return Internal::hr(); }

	/// @brief Returns a std::string customized horizontal ruler. Intended to be used when printing to the console.
	/// @param mOffset Length offset of the ruler.
	/// @param mChar Character used to create the ruler.
	inline std::string hr(int mOffset, char mChar = '_') { return Internal::hr(mOffset, mChar); }

	/// @brief Starts or stops suppressing the log functionality.
	/// @details While the log is being suppressed, no output will be given to either `std::cout` or to the file log stream.
	inline void setLogSuppressed(bool mLogSuppressed) noexcept { Internal::getLogSuppressed() = mLogSuppressed; }

	/// @brief Returns a `LOut`-like log object that uses the same syntax but does nothing.
	inline auto loNull() noexcept { return Internal::NullLOut{}; }

	/// @brief Returns a `LOut`-like log object that uses the same syntax but does nothing.
	template<typename T> inline auto loNull(const T&) noexcept { return Internal::NullLOut{}; }
}

#endif
