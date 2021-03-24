env_add_dep(test)

env_fetch(
        test
        GIT_REPOSITORY https://github.com/onqtam/doctest.git
        GIT_TAG 2.4.6
)

env_target_link(test_dep INTERFACE doctest)
