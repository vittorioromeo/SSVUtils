// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_CORE_ASSERT
#define SSVU_CORE_ASSERT

/// @macro Static assertion. Checked at compile-time.
/// @details Wrapper for the standard `static_assert`.
#define SSVU_ASSERT_STATIC(...) static_assert(__VA_ARGS__)

/// @macro Static assertion. Checked at compile-time. No message.
/// @details Wrapper for the standard `static_assert` with an empty string message.
#define SSVU_ASSERT_STATIC_NM(...) static_assert(__VA_ARGS__, "")

// `SSVU_ASSERT_FORCE_OFF` and `SSVU_ASSERT_FORCE_ON` macros force enabling/disabling of assertions.
// `SSVU_ASSERT_FORCE_ON` has priority over `SSVU_ASSERT_FORCE_OFF`.

#if (defined(SSVU_ASSERT_FORCE_OFF) || !defined(SSVU_DEBUG))
	#define SSVU_IMPL_ASSERT_DISABLED 1
#else
	#define SSVU_IMPL_ASSERT_DISABLED 0
#endif

#if (defined(SSVU_ASSERT_FORCE_ON))
	#define SSVU_IMPL_ASSERT_DISABLED 0
#endif

#if SSVU_IMPL_ASSERT_DISABLED
	/// @macro Assertion in release mode: this macro does nothing.
	#define SSVU_ASSERT(...)

	/// @macro Assertion in release mode: this macro does nothing.
	#define SSVU_ASSERT_CONSTEXPR(...)
#else
	namespace ssvu
	{
		namespace Internal
		{
			/// @brief Internal struct storing data for an assertion.
			struct AssertData { std::string code, line, file; };

			/// @brief Internal struct storing global state for assertions.
			struct AssertState { bool skip{false}; };

			/// @brief Returns a reference to the global thread_local AssertState instance.
			inline auto& getAssertState() noexcept { thread_local AssertState result; return result; }

			/// @brief Assert implementation: if mExpression is false, the assertion fires.
			/// @details Called via the SSVU_ASSERT macro.
			void assertImpl(AssertData&& mAD, bool mExpression, const std::string& mMsg = "") noexcept;
		}
	}

	/// @macro Normal assertion. Requires a boolean expression and an optional string message.
	///	@details If the expression returns false, the assertion fires, calling `ssvu::Internal::assertImpl`.
	#define SSVU_ASSERT(...) \
		do \
		{ \
			::ssvu::Internal::AssertData ad{}; \
			ad.code = SSVPP_TOSTR_SEP(",", SSVPP_EMPTY(), __VA_ARGS__); \
			ad.line = SSVPP_TOSTR(__LINE__); \
			ad.file = __FILE__; \
			::ssvu::Internal::assertImpl(std::move(ad), __VA_ARGS__); \
		} while(false)

	// TODO: BUG: gcc - doesn't work yet
	/// @macro Constexpr assertion. Work-in-progress.
	#if SSVU_COMPILER_CLANG
		/// @macro When compiling with clang, using `assert` in constexpr functions seems to be allowed.
		#define SSVU_ASSERT_CONSTEXPR(...) assert(__VA_ARGS__)
	#else
		/// @macro When compiling with g++, using `assert` in constexpr functions causes an error.
		#define SSVU_ASSERT_CONSTEXPR(...)
	#endif
#endif

#endif
