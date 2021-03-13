#ifndef ENV_NAMESPACING_HPP
#define ENV_NAMESPACING_HPP

// format, detail, test

#define FWA_FORMAT_NAMESPACE \
    namespace                \
    {                        \
    }
#define FWA_NAMESPACE_BEGIN(_name) \
    namespace _name                \
    {                              \
        FWA_FORMAT_NAMESPACE
#define FWA_NAMESPACE_END() \
    FWA_FORMAT_NAMESPACE    \
    }

#define FWA_NAMESPACE_DETAIL_BEGIN FWA_NAMESPACE_BEGIN(detail)
#define FWA_NAMESPACE_DETAIL_END FWA_NAMESPACE_END()

#define FWA_NAMESPACE_TEST_BEGIN FWA_NAMESPACE_BEGIN(test)
#define FWA_NAMESPACE_TEST_END FWA_NAMESPACE_END()

// fwa namespaces

#define FWA_NAMESPACE_CORE_BEGIN \
    namespace fwa                \
    {                            \
        inline namespace core    \
        {                        \
            FWA_FORMAT_NAMESPACE
#define FWA_NAMESPACE_CORE_END \
    FWA_FORMAT_NAMESPACE       \
    }                          \
    }
#define FWA_CORE ::fwa::core

#define FWA_NAMESPACE_GRAPH_BEGIN \
    namespace fwa                 \
    {                             \
        namespace graph           \
        {                         \
            FWA_FORMAT_NAMESPACE
#define FWA_NAMESPACE_GRAPH_END \
    FWA_FORMAT_NAMESPACE        \
    }                           \
    }
#define FWA_GRAPH ::fwa::graph

#define FWA_NAMESPACE_EXEC_BEGIN \
    namespace fwa                \
    {                            \
        namespace exec           \
        {                        \
            FWA_FORMAT_NAMESPACE
#define FWA_NAMESPACE_EXEC_END \
    FWA_FORMAT_NAMESPACE       \
    }                          \
    }
#define FWA_EXEC ::fwa::exec

// core -> global

#define FWA_NAMESPACE_GLOBAL_BEGIN FWA_NAMESPACE_CORE_END FWA_FORMAT_NAMESPACE
#define FWA_NAMESPACE_GLOBAL_END FWA_FORMAT_NAMESPACE FWA_NAMESPACE_CORE_BEGIN
#define FWA_GLOBAL ::

// core -> std

#define FWA_NAMESPACE_STD_BEGIN          \
    FWA_NAMESPACE_CORE_END namespace std \
    {                                    \
        FWA_FORMAT_NAMESPACE
#define FWA_NAMESPACE_STD_END \
    FWA_FORMAT_NAMESPACE      \
    }                         \
    FWA_NAMESPACE_CORE_BEGIN
#define FWA_STD ::std

// doctest

#define FWA_DOCT ::doctest

#endif // ENV_NAMESPACING_HPP
