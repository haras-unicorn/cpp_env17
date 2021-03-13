#ifndef ENV_NAMESPACING_HPP
#define ENV_NAMESPACING_HPP

// namespaces

#define ENV_FORMAT_NAMESPACE \
    namespace                \
    {                        \
    }

#define ENV_CUSTOM_NAMESPACE_BEGIN(_name) \
    namespace _name                       \
    {                                     \
        ENV_FORMAT_NAMESPACE

#define ENV_CUSTOM_NAMESPACE_END() \
    ENV_FORMAT_NAMESPACE           \
    }

// format, detail, test

#define ENV_NAMESPACE_DETAIL_BEGIN ENV_CUSTOM_NAMESPACE_BEGIN(detail)
#define ENV_NAMESPACE_DETAIL_END ENV_CUSTOM_NAMESPACE_END()

#define ENV_NAMESPACE_TEST_BEGIN ENV_CUSTOM_NAMESPACE_BEGIN(test)
#define ENV_NAMESPACE_TEST_END ENV_CUSTOM_NAMESPACE_END()

#define ENV_NAMESPACE_BENCH_BEGIN ENV_CUSTOM_NAMESPACE_BEGIN(bench)
#define ENV_NAMESPACE_BENCH_END ENV_CUSTOM_NAMESPACE_END()

// env namespaces

#define ENV_NAMESPACE_BEGIN ENV_CUSTOM_NAMESPACE_BEGIN(env)
#define ENV_NAMESPACE_END ENV_CUSTOM_NAMESPACE_END()
#define ENV ::env

// core -> global

#define ENV_NAMESPACE_GLOBAL_BEGIN ENV_NAMESPACE_CORE_END ENV_FORMAT_NAMESPACE
#define ENV_NAMESPACE_GLOBAL_END ENV_FORMAT_NAMESPACE ENV_NAMESPACE_CORE_BEGIN
#define ENV_GLOBAL ::

// core -> std

#define ENV_NAMESPACE_STD_BEGIN \
    ENV_NAMESPACE_END           \
    ENV_CUSTOM_NAMESPACE_BEGIN(std)
#define ENV_NAMESPACE_STD_END  \
    ENV_CUSTOM_NAMESPACE_END() \
    ENV_NAMESPACE_BEGIN
#define ENV_STD ::std

// doctest, benchmark

#define ENV_DTEST ::doctest
#define ENV_GBENCH ::benchmark

#endif // ENV_NAMESPACING_HPP
