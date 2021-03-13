env_add_dependency(default INTERFACE)

env_target_include(default INTERFACE ${PROJECT_SOURCE_DIR}/src)
env_target_link(default INTERFACE env_std env_boost)

env_target_compile(default INTERFACE cxx_std_17)
