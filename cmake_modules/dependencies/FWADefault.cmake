fwa_add_dependency(default INTERFACE)

fwa_target_include(default INTERFACE ${PROJECT_SOURCE_DIR}/src)
fwa_target_link(default INTERFACE fwa_std fwa_boost)

fwa_target_compile(default INTERFACE cxx_std_17)
