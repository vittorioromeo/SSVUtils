// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_UTILS_ASSERT
#define SSVU_UTILS_ASSERT

#include <string>
#include <iostream>
#include "SSVUtils/Preprocessor/Preprocessor.hpp"
#include "SSVUtils/Log/Log.hpp"

namespace ssvu
{
	namespace Internal
	{
		struct AssertData { std::string code, line, file; };

		inline AssertData& getAssertData() noexcept { static AssertData result; return result; }

		inline void assertImpl(bool mExpression, const std::string& mMsg = "") noexcept
		{
			if(mExpression) return;

			ssvu::lo() << "\n";
			ssvu::lo("ASSERTION FAILED") << mMsg << "\n\n"
				<< "Line " << getAssertData().line << " in file " << getAssertData().file << "\n"
				<< "Code: " << getAssertData().code << "\n" << std::endl;

			std::terminate();
		}
	}

	#ifdef NDEBUG
		#define SSVU_ASSERT(...)
		#define SSVU_ASSERT_STATIC(...)
		#define SSVU_ASSERT_CONSTEXPR(...)
	#else
		#define SSVU_ASSERT(...) 			\
			do \
			{ \
				ssvu::Internal::getAssertData().code = SSVPP_TOSTR_SEP(",", SSVPP_EMPTY(), __VA_ARGS__); \
				ssvu::Internal::getAssertData().line = SSVPP_TOSTR(__LINE__); \
				ssvu::Internal::getAssertData().file = __FILE__; \
				ssvu::Internal::assertImpl(__VA_ARGS__); \
			} while(false);

		#define SSVU_ASSERT_STATIC(...)		static_assert(__VA_ARGS__)
		#define SSVU_ASSERT_CONSTEXPR(...)

		// C++14: uncomment
		/*
		#ifdef __clang__
			/// @macro When compiling with clang, using `assert` in constexpr functions seems to be allowed.
			#define SSVU_ASSERT_CONSTEXPR(mTest) assert(mTest)
		#else
			/// @macro When compiling with g++, using `assert` in constexpr functions causes an error.
			#define SSVU_ASSERT_CONSTEXPR(mTest)
		#endif
		*/
	#endif
}

#endif
