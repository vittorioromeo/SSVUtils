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
inline void test_assert_expected(
bool x, const T& res, const char* expr, const char* expected)
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

template <typename T>
inline void test_assert_ns(bool x, const T&, const char* expr) noexcept
{
    using namespace std::literals;

    if(x) return;
    std::ostringstream error;

    error << expr << "\n";

    std::cout << error.str() << std::endl;
    std::terminate();
}

template <typename T>
inline void test_assert_ns_expected(
bool x, const T&, const char* expr, const char* expected)
{
    using namespace std::literals;

    if(x) return;
    std::ostringstream error;

    error << expr << "\n"
          << "expected: " << expected << "\n";

    std::cout << error.str() << std::endl;
    std::terminate();
}
}

#define TEST_ASSERT(expr)                     \
    do                                        \
    {                                         \
        auto _t_x = expr;                     \
        impl::test_assert(_t_x, expr, #expr); \
    } while(false)

#define TEST_ASSERT_OP(lhs, op, rhs)                                  \
    do                                                                \
    {                                                                 \
        auto _t_xl = lhs;                                             \
        auto _t_x = _t_xl op rhs;                                     \
        impl::test_assert_expected(_t_x, _t_xl, #lhs #op #rhs, #rhs); \
    } while(false)

#define TEST_ASSERT_NS(expr)                     \
    do                                           \
    {                                            \
        auto _t_x = expr;                        \
        impl::test_assert_ns(_t_x, expr, #expr); \
    } while(false)

#define TEST_ASSERT_NS_OP(lhs, op, rhs)                                  \
    do                                                                   \
    {                                                                    \
        auto _t_xl = lhs;                                                \
        auto _t_x = _t_xl op rhs;                                        \
        impl::test_assert_ns_expected(_t_x, _t_xl, #lhs #op #rhs, #rhs); \
    } while(false)
