// Copyright (c) 2013-2015 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_TEST_HPP
#define SSVU_TEST_HPP

#include "SSVUtils/Core/Core.hpp"

/// @macro Generates an unique name for the test class type.
#define SSVUT_IMPL_GET_NAME_TYPE(mName) \
    VRM_PP_CAT(SSVUTTestUnique, mName, __LINE__)

#ifndef SSVUT_DISABLE
#include "SSVUtils/Test/Internal/TestImplEnabled.hpp"

/// @macro Generates an unique name for the test runner instance.
#define SSVUT_IMPL_GET_NAME_RUNNER(mName) \
    VRM_PP_CAT(SSVUT_IMPL_GET_NAME_TYPE(mName), runner)

/// @macro Generates an unique key for the test.
#define SSVUT_IMPL_GET_KEY(mName) VRM_PP_TOSTR(SSVUT_IMPL_GET_NAME_TYPE(mName))

/// @macro Generates the test struct.
#define SSVUT_IMPL_GENERATE_STRUCT(mName)                       \
    struct SSVUT_IMPL_GET_NAME_TYPE(mName) final                \
    : public ::ssvu::Test::Impl::TestBase                       \
    {                                                           \
        inline SSVUT_IMPL_GET_NAME_TYPE(mName)()                \
            : ::ssvu::Test::Impl::TestBase{VRM_PP_TOSTR(mName), \
              VRM_PP_TOSTR(__LINE__), VRM_PP_TOSTR(__FILE__)}   \
        {                                                       \
        }                                                       \
        virtual void run() const override;                      \
    };

/// @macro Generates the test runner.
#define SSVUT_IMPL_GENERATE_RUNNER(mName)                                  \
    static ::ssvu::Test::Impl::Runner SSVUT_IMPL_GET_NAME_RUNNER(mName){[] \
    {                                                                      \
        if(::ssvu::Test::Impl::wasTestExecuted(SSVUT_IMPL_GET_KEY(mName))) \
            return;                                                        \
        ::ssvu::Test::Impl::setTestExecuted(SSVUT_IMPL_GET_KEY(mName));    \
        ssvu::getEmplaceUPtr<SSVUT_IMPL_GET_NAME_TYPE(mName)>(             \
        ::ssvu::Test::Impl::getTestStorage());                             \
    }};

/// @macro Test wrapper. After using this macro, write the test body in curly
/// braces.
#define SSVUT_TEST(mName)             \
    SSVUT_IMPL_GENERATE_STRUCT(mName) \
    SSVUT_IMPL_GENERATE_RUNNER(mName) \
    inline void SSVUT_IMPL_GET_NAME_TYPE(mName)::run() const


/// @macro Test check. If `mExpr mOp mRes` is false, the test fails.
#define SSVUT_EXPECT_OP(mExpr, mOp, mRes)                 \
    do                                                    \
    {                                                     \
        if(SSVU_UNLIKELY(!(mExpr mOp mRes))) {            \
            ::ssvu::Test::Impl::testFailure(this, #mExpr, \
            VRM_PP_TOSTR(__LINE__), ::ssvu::toStr(mExpr), \
            ::ssvu::toStr(mRes));                         \
        }                                                 \
    } while(false)

/// @macro Test check. If `mExpr` is false, the test fails.
#define SSVUT_EXPECT(mExpr) SSVUT_EXPECT_OP((mExpr), ==, true)

/// @macro Runs all tests.
#define SSVUT_RUN()                        \
    do                                     \
    {                                      \
        ::ssvu::Test::Impl::runAllTests(); \
    } while(false)

#else
#define SSVUT_TEST(mName) \
    inline void VRM_PP_CAT(SSVUT_IMPL_GET_NAME_TYPE(mName), unused)()
#define SSVUT_EXPECT(...)
#define SSVUT_EXPECT_OP(...)
#define SSVUT_RUN() \
    while(false) {  \
    }
#endif

#endif
