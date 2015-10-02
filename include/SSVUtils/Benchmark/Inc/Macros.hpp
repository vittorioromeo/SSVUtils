// Copyright (c) 2013-2015 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_IMPL_BENCHMARK_MACROS
#define SSVU_IMPL_BENCHMARK_MACROS

/// @macro Instantiates a `LogScopeExit` temp-named object in the current scope.
/// @details The istantiated object will run a benchmark during its lifetime.
/// Pass the desired benchmark title as a parameter. Requires semicolon.
#define SSVU_BENCHMARK_LOG_SCOPE_EXIT(...)                                     \
    ::ssvu::Benchmark::Impl::LogScopeExit VRM_PP_CAT(__logScopeExit, __LINE__) \
    {                                                                          \
        __VA_ARGS__                                                            \
    }

/// @macro Instantiates a `RunGroupScopeExit` temp-named object in the current
/// scope.
/// @details The istantiated object will resume and pause a group benchmark
/// during its lifetime.
/// Pass the desired benchmark group as a parameter. Requires semicolon.
#define SSVU_BENCHMARK_RUN_GROUP_SCOPE_EXIT(...)           \
    ::ssvu::Benchmark::Impl::RunGroupScopeExit VRM_PP_CAT( \
    __logGroupScopeExit, __LINE__)                         \
    {                                                      \
        __VA_ARGS__                                        \
    }

/// @macro Instantiates a `InitGroupScopeExit` temp-named object in the current
/// scope.
/// @details The istantiated object will reset and endLo a group benchmark
/// during its lifetime.
/// Pass the desired benchmark group as a parameter. Requires semicolon.
#define SSVU_BENCHMARK_INIT_GROUP_SCOPE_EXIT(...)           \
    ::ssvu::Benchmark::Impl::InitGroupScopeExit VRM_PP_CAT( \
    __initGroupScopeExit, __LINE__)                         \
    {                                                       \
        __VA_ARGS__                                         \
    }

#endif
