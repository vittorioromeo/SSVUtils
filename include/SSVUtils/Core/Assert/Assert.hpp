// Copyright (c) 2013-2015 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_CORE_ASSERT
#define SSVU_CORE_ASSERT

/// @macro Static assertion. Checked at compile-time.
/// @details Wrapper for the standard `static_assert`.
#define SSVU_ASSERT_STATIC(...) static_assert(__VA_ARGS__)

/// @macro Static assertion. Checked at compile-time. No message.
/// @details Wrapper for the standard `static_assert` with a default message.
#define SSVU_ASSERT_STATIC_NM(...) static_assert(__VA_ARGS__, SSVPP_TOSTR(__VA_ARGS__))

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
	#define SSVU_ASSERT_OP(...)

	/// @macro Assertion in release mode: this macro does nothing.
	#define SSVU_ASSERT_OP_MSG(...)

	/// @macro Assertion in release mode: this macro does nothing.
	#define SSVU_ASSERT_CONSTEXPR(...)
#else
	/// @macro Defined if asserts are enabled. Should be used for user-code checks.
	#define SSVU_ASSERT_IS_ENABLED 1

	namespace ssvu
	{
		namespace Impl
		{
			/// @brief Internal struct storing data for an assertion.
			struct AssertData { std::string code, line, file, lhs, rhs; };

			/// @brief Internal struct storing global state for assertions.
			struct AssertState { bool skip{false}; };

			/// @brief Returns a reference to the global thread_local AssertState instance.
			inline auto& getAssertState() noexcept { thread_local AssertState result; return result; }

			/// @brief Assert implementation: if mExpression is false, the assertion fires.
			/// @details Called via the SSVU_ASSERT macro.
			void assertImpl(AssertData&& mAD, bool mExpression, const std::string& mMsg = "") noexcept;
		}
	}

	/// @macro Shared implementation details for assertions.
	#define SSVU_IMPL_ASSERT_INIT(...) \
		::ssvu::Impl::AssertData ad{}; \
		ad.code = SSVPP_SEP_TOSTR(" ", SSVPP_EMPTY(), __VA_ARGS__); \
		ad.line = SSVPP_TOSTR(__LINE__); \
		ad.file = __FILE__;

	/// @macro Normal assertion. Requires a boolean expression and an optional string message.
	///	@details If the expression returns false, the assertion fires, calling `ssvu::Impl::assertImpl`.
	#define SSVU_ASSERT(...) \
		do \
		{ \
			SSVU_IMPL_ASSERT_INIT(__VA_ARGS__) \
			::ssvu::Impl::assertImpl(::ssvu::mv(ad), __VA_ARGS__); \
		} while(false)

	/// @macro Assertion that checks the result of an operation between `mLhs` and `mRhs`.
	/// @details Displays a message when fired.
	#define SSVU_ASSERT_OP_MSG(mLhs, mOp, mRhs, ...) \
		do \
		{ \
			SSVU_IMPL_ASSERT_INIT(mLhs, mOp, mRhs) \
			ad.lhs = SSVPP_TOSTR(mLhs) " = " + ::ssvu::toStr(mLhs); \
			ad.rhs = SSVPP_TOSTR(mRhs) " = " + ::ssvu::toStr(mRhs); \
			::ssvu::Impl::assertImpl(::ssvu::mv(ad), mLhs mOp mRhs, __VA_ARGS__); \
		} while(false)

	/// @macro Assertion that checks the result of an operation between `mLhs` and `mRhs`.
	/// @details No message.
	#define SSVU_ASSERT_OP(...) SSVU_ASSERT_OP_MSG(__VA_ARGS__, "")

	/// @macro Constexpr assertion.
	#define SSVU_ASSERT_CONSTEXPR(...) assert(__VA_ARGS__)
#endif

#endif
