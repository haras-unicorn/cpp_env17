env_add_dep(default)

env_target_include(default_dep INTERFACE ${PROJECT_SOURCE_DIR}/src)
env_target_link(default_dep INTERFACE env_std_dep env_boost_dep env_alloc_dep env_test_dep env_bench_dep)

env_target_compile(default_dep INTERFACE cxx_std_17)
