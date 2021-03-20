env_add_dep(test)

env_target_link(test_dep INTERFACE CONAN_PKG::doctest)
