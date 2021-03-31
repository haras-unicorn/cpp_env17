#ifndef ENV_NAMESPACING_HPP
#define ENV_NAMESPACING_HPP


// namespaces

#define ENV_FORMAT_NAMESPACE namespace { }
#define ENV_CUSTOM_NAMESPACE_BEGIN(_name)  namespace _name { ENV_FORMAT_NAMESPACE
#define ENV_CUSTOM_NAMESPACE_END() ENV_FORMAT_NAMESPACE }


// env namespace

#ifndef ENV_NAMESPACE
#define ENV_NAMESPACE env
#endif


// namespace scope

#define ENV_DETAIL_BEGIN ENV_CUSTOM_NAMESPACE_BEGIN(detail)
#define ENV_DETAIL_END ENV_CUSTOM_NAMESPACE_END()

#define ENV_TEST_BEGIN ENV_CUSTOM_NAMESPACE_BEGIN(test)
#define ENV_TEST_END ENV_CUSTOM_NAMESPACE_END()

#define ENV_BENCH_BEGIN ENV_CUSTOM_NAMESPACE_BEGIN(bench)
#define ENV_BENCH_END ENV_CUSTOM_NAMESPACE_END()

#define ENV_BEGIN ENV_CUSTOM_NAMESPACE_BEGIN(ENV_NAMESPACE) ENV_USING
#define ENV_END ENV_CUSTOM_NAMESPACE_END()

#define ENV_GLOBAL_BEGIN ENV_END ENV_FORMAT_NAMESPACE
#define ENV_GLOBAL_END ENV_FORMAT_NAMESPACE ENV_BEGIN

#define ENV_STD_BEGIN ENV_END ENV_CUSTOM_NAMESPACE_BEGIN(std)
#define ENV_STD_END ENV_CUSTOM_NAMESPACE_END() ENV_BEGIN


// namespace entries

#define ENV_GLOBAL
#define ENV ENV_GLOBAL::ENV_NAMESPACE
#define ENV_STD ENV_GLOBAL::std
#define ENV_CHRONO ENV_STD::chrono
#define ENV_DTEST ENV_GLOBAL::doctest
#define ENV_GBENCH ENV_GLOBAL::benchmark
#define ENV_HANA ENV_GLOBAL::boost::hana


#endif // ENV_NAMESPACING_HPP
