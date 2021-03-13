#ifndef ENV_NAMESPACING_HPP
#define ENV_NAMESPACING_HPP

// format, detail, test

#define ENV_FORMAT_NAMESPACE \
    namespace                \
    {                        \
    }
#define ENV_NAMESPACE_BEGIN(_name) \
    namespace _name                \
    {                              \
        ENV_FORMAT_NAMESPACE
#define ENV_NAMESPACE_END() \
    ENV_FORMAT_NAMESPACE    \
    }

#define ENV_NAMESPACE_DETAIL_BEGIN ENV_NAMESPACE_BEGIN(detail)
#define ENV_NAMESPACE_DETAIL_END ENV_NAMESPACE_END()

#define ENV_NAMESPACE_TEST_BEGIN ENV_NAMESPACE_BEGIN(test)
#define ENV_NAMESPACE_TEST_END ENV_NAMESPACE_END()

// fwa namespaces

#define ENV_NAMESPACE_CORE_BEGIN \
    namespace fwa                \
    {                            \
        inline namespace core    \
        {                        \
            ENV_FORMAT_NAMESPACE
#define ENV_NAMESPACE_CORE_END \
    ENV_FORMAT_NAMESPACE       \
    }                          \
    }
#define ENV ::fwa::core

#define ENV_NAMESPACE_GRAPH_BEGIN \
    namespace fwa                 \
    {                             \
        namespace graph           \
        {                         \
            ENV_FORMAT_NAMESPACE
#define ENV_NAMESPACE_GRAPH_END \
    ENV_FORMAT_NAMESPACE        \
    }                           \
    }
#define ENV_GRAPH ::fwa::graph

#define ENV_NAMESPACE_EXEC_BEGIN \
    namespace fwa                \
    {                            \
        namespace exec           \
        {                        \
            ENV_FORMAT_NAMESPACE
#define ENV_NAMESPACE_EXEC_END \
    ENV_FORMAT_NAMESPACE       \
    }                          \
    }
#define ENV_EXEC ::fwa::exec

// core -> global

#define ENV_NAMESPACE_GLOBAL_BEGIN ENV_NAMESPACE_CORE_END ENV_FORMAT_NAMESPACE
#define ENV_NAMESPACE_GLOBAL_END ENV_FORMAT_NAMESPACE ENV_NAMESPACE_CORE_BEGIN
#define ENV_GLOBAL ::

// core -> std

#define ENV_NAMESPACE_STD_BEGIN          \
    ENV_NAMESPACE_CORE_END namespace std \
    {                                    \
        ENV_FORMAT_NAMESPACE
#define ENV_NAMESPACE_STD_END \
    ENV_FORMAT_NAMESPACE      \
    }                         \
    ENV_NAMESPACE_CORE_BEGIN
#define ENV_STD ::std

// doctest

#define ENV_DOCT ::doctest

#endif // ENV_NAMESPACING_HPP
