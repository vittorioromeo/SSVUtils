// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_TEST_HPP
#define SSVU_TEST_HPP

#include "SSVUtils/Core/Core.hpp"

/// @macro Generates an unique name for the test class type.
#define SSVUT_IMPL_GET_NAME_TYPE(mName) SSVPP_CAT(SSVUTTestUnique, mName, __LINE__)

/// @macro Generates an unique name for the test runner instance.
#define SSVUT_IMPL_GET_NAME_RUNNER(mName) SSVPP_CAT(SSVUT_IMPL_GET_NAME_TYPE(mName), runner)

/// @macro Generates an unique name for the test instance + test class type.
#define SSVUT_IMPL_GET_NAME_INSTANCE(mName) SSVPP_CAT(SSVUT_IMPL_GET_NAME_TYPE(mName), instance)

/// @macro Generates an unique key for the test.
#define SSVUT_IMPL_GET_KEY(mName) SSVPP_TOSTR(SSVUT_IMPL_GET_NAME_TYPE(mName))

/// @macro Generates the test struct.
#define SSVUT_IMPL_GENERATE_STRUCT(mName) \
	struct SSVUT_IMPL_GET_NAME_TYPE(mName) final : public ::ssvu::Test::Internal::TestBase \
	{ \
		inline SSVUT_IMPL_GET_NAME_TYPE(mName) () : ::ssvu::Test::Internal::TestBase{SSVPP_TOSTR(mName), SSVPP_TOSTR(__LINE__), SSVPP_TOSTR(__FILE__)} { } \
		virtual void run() const override; \
	}; \
	static SSVUT_IMPL_GET_NAME_TYPE(mName) SSVUT_IMPL_GET_NAME_INSTANCE(mName);

/// @macro Generates the test runner.
#define SSVUT_IMPL_GENERATE_RUNNER(mName) \
	static ::ssvu::Test::Internal::Runner SSVUT_IMPL_GET_NAME_RUNNER(mName) {[] \
	{ \
		if(::ssvu::Test::Internal::wasTestExecuted(SSVUT_IMPL_GET_KEY(mName))) return; \
		::ssvu::Test::Internal::setTestExecuted(SSVUT_IMPL_GET_KEY(mName)); \
		ssvu::getEmplaceUPtr<SSVUT_IMPL_GET_NAME_TYPE(mName)>(::ssvu::Test::Internal::getTestStorage(), SSVUT_IMPL_GET_NAME_INSTANCE(mName)); \
	}};

#ifndef SSVUT_DISABLE
	/// @macro Test wrapper. After using this macro, write the test body in curly braces.
	#define SSVUT_TEST(mName) \
		SSVUT_IMPL_GENERATE_STRUCT(mName) \
		SSVUT_IMPL_GENERATE_RUNNER(mName) \
		inline void SSVUT_IMPL_GET_NAME_TYPE(mName)::run() const

	/// @macro Test check. If `mExpr` is false, the test fails.
	#define SSVUT_EXPECT(mExpr) \
		do \
		{ \
			if(!(mExpr)) throw ::ssvu::Test::Internal::TestFailException{this, #mExpr, SSVPP_TOSTR(__LINE__), ::ssvu::toStr(mExpr)}; \
		} while(false)

	/// @macro Test check. If `mExpr mOp mRes` is false, the test fails.
	#define SSVUT_EXPECT_OP(mExpr, mOp, mRes) \
		do \
		{ \
			if(!(mExpr mOp mRes)) throw ::ssvu::Test::Internal::TestFailException{this, #mExpr, SSVPP_TOSTR(__LINE__), ::ssvu::toStr(mExpr), ::ssvu::toStr(mRes)}; \
		} while(false)

	/// @macro Runs all tests.
	#define SSVUT_RUN() ::ssvu::Test::Internal::runAllTests()

	#include "SSVUtils/Test/Internal/TestImplEnabled.hpp"
#else
	#define SSVUT_TEST(mName) inline void SSVPP_CAT(SSVUT_IMPL_GET_NAME_TYPE(mName), unused) ()
	#define SSVUT_EXPECT(...)
	#define SSVUT_RUN() while(false){ }
#endif

#endif
