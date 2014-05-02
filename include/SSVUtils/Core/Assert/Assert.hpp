// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_CORE_ASSERT
#define SSVU_CORE_ASSERT

#include <string>
#include <iostream>
#include "SSVUtils/Core/Preprocessor/Preprocessor.hpp"

namespace ssvu
{
	namespace Internal
	{
		/// @brief Internal struct storing data and state for assertions.
		struct AssertState
		{
			#ifndef NDEBUG
				std::string code, line, file;
				bool skip{false};
			#endif
		};

		/// @brief Returns a reference to the global static AssertState instance.
		inline AssertState& getAssertState() noexcept { static AssertState result; return result; }

		/// @brief Assert implementation: if mExpression is false, the assertion fires.
		/// @details Called via the SSVU_ASSERT macro.
		void assertImpl(bool mExpression, const std::string& mMsg = "") noexcept;
	}

	/// @macro Static assertion. Checked at compile-time.
	/// @details Currently, it is only a wrapper for the standard `static_assert`.
	#define SSVU_ASSERT_STATIC(...)	static_assert(__VA_ARGS__)

	#ifdef NDEBUG
		/// @macro Assertion in release mode: this macro does nothing.
		#define SSVU_ASSERT(...)

		/// @macro Assertion in release mode: this macro does nothing.
		#define SSVU_ASSERT_CONSTEXPR(...)
	#else
		/// @macro Normal assertion. Requires a boolean expression and an optional string message.
		///	@details If the expression returns false, the assertion fires, calling `ssvu::Internal::assertImpl`.
		#define SSVU_ASSERT(...) \
			do \
			{ \
				ssvu::Internal::getAssertState().code = SSVPP_TOSTR_SEP(",", SSVPP_EMPTY(), __VA_ARGS__); \
				ssvu::Internal::getAssertState().line = SSVPP_TOSTR(__LINE__); \
				ssvu::Internal::getAssertState().file = __FILE__; \
				ssvu::Internal::assertImpl(__VA_ARGS__); \
			} while(false)

		// TODO:
		/// @macro Constexpr assertion. Work-in-progress.
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
