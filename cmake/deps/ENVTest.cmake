env_add_dep(test)

env_fetch(
        test
        GIT_REPOSITORY https://github.com/onqtam/doctest.git
        GIT_TAG 2.4.5
        DOWNLOAD_DIR ${DOWNLOAD_DIRECTORY}
)

env_target_link(test_dep INTERFACE doctest)
