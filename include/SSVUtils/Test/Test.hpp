// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_TEST_HPP
#define SSVU_TEST_HPP

#include "SSVUtils/Core/Core.hpp"

#define SSVUT_IMPL_GET_NAME_TYPE(mName)		SSVPP_CAT(SSVUTTestUnique, mName, __LINE__)
#define SSVUT_IMPL_GET_NAME_RUNNER(mName)	SSVPP_CAT(SSVUT_IMPL_GET_NAME_TYPE(mName), runner)
#define SSVUT_IMPL_GET_NAME_INSTANCE(mName)	SSVPP_CAT(SSVUT_IMPL_GET_NAME_TYPE(mName), instance)
#define SSVUT_IMPL_GET_KEY(mName)			SSVPP_TOSTR(SSVUT_IMPL_GET_NAME_TYPE(mName))

#define SSVUT_IMPL_GENERATE_STRUCT(mName) \
	struct SSVUT_IMPL_GET_NAME_TYPE(mName) final : public ::ssvu::Test::Internal::TestBase \
	{ \
		inline SSVUT_IMPL_GET_NAME_TYPE(mName) () : ::ssvu::Test::Internal::TestBase{SSVPP_TOSTR(mName), SSVPP_TOSTR(__LINE__), SSVPP_TOSTR(__FILE__)} { } \
		virtual void run() const override; \
	}; \
	static SSVUT_IMPL_GET_NAME_TYPE(mName) SSVUT_IMPL_GET_NAME_INSTANCE(mName);

#define SSVUT_IMPL_GENERATE_RUNNER(mName) \
	static ::ssvu::Test::Internal::Runner SSVUT_IMPL_GET_NAME_RUNNER(mName) {[] \
	{ \
		if(::ssvu::Test::Internal::wasTestExecuted(SSVUT_IMPL_GET_KEY(mName))) return; \
		::ssvu::Test::Internal::setTestExecuted(SSVUT_IMPL_GET_KEY(mName)); \
		ssvu::getEmplaceUptr<SSVUT_IMPL_GET_NAME_TYPE(mName)>(::ssvu::Test::Internal::getTestStorage(), SSVUT_IMPL_GET_NAME_INSTANCE(mName)); \
	}};

#ifndef SSVUT_DISABLE
	#define SSVUT_TEST(mName) \
		SSVUT_IMPL_GENERATE_STRUCT(mName) \
		SSVUT_IMPL_GENERATE_RUNNER(mName) \
		inline void SSVUT_IMPL_GET_NAME_TYPE(mName)::run() const

	#define SSVUT_EXPECT(mExpr) \
		while(true) \
		{ \
			if(!(mExpr)) throw ::ssvu::Test::Internal::TestFailException{this, SSVPP_TOSTR_SEP(",", SSVPP_EMPTY(), mExpr), SSVPP_TOSTR(__LINE__)}; \
			break; \
		}

	#define SSVUT_RUN() ::ssvu::Test::Internal::runAllTests()

	#include "SSVUtils/Test/Internal/TestImplEnabled.hpp"
#else
	#define SSVUT_TEST(mName) inline void SSVPP_CAT(SSVUT_IMPL_GET_NAME_TYPE(mName), unused) ()
	#define SSVUT_EXPECT(...)
	#define SSVUT_RUN() while(false){ }
#endif

#endif
