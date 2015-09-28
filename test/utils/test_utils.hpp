#pragma once

#include <exception>
#include <string>
#include <sstream>
#include <iostream>

namespace impl
{
    template <typename T>
    inline void test_assert(bool x, const T& res, const char* expr) noexcept
    {
        using namespace std::literals;

        if(x) return;
        std::ostringstream error;

        error << expr << "\n"
              << "result: " << res << "\n";

        std::cout << error.str() << std::endl;
        std::terminate();
    }

    template <typename T>
    inline void test_assert_expected(bool x, const T& res, const char* expr,
                                     const char* expected)
    {
        using namespace std::literals;

        if(x) return;
        std::ostringstream error;

        error << expr << "\n"
              << "result: " << res << "\n"
              << "expected: " << expected << "\n";

        std::cout << error.str() << std::endl;
        std::terminate();
    }
}

#define TEST_ASSERT(expr)                     \
    do                                        \
    {                                         \
        impl::test_assert(expr, expr, #expr); \
    } while(false)

#define TEST_ASSERT_OP(lhs, op, rhs)                                      \
    do                                                                    \
    {                                                                     \
        impl::test_assert_expected(lhs op rhs, lhs, #lhs #op #rhs, #rhs); \
    } while(false)
