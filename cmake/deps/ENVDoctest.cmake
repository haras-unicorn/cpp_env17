env_fetch(
        doctest
        GIT_REPOSITORY https://github.com/onqtam/doctest.git
        GIT_TAG 2.4.5
        DOWNLOAD_DIR ${DOWNLOAD_DIRECTORY})

env_add_dependency(doctest INTERFACE)
env_target_link(doctest INTERFACE doctest)
