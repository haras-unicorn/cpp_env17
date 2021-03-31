env_add_dep(boost)

env_fetch(
        hana
        GIT_REPOSITORY https://github.com/boostorg/hana.git
        GIT_TAG v1.7.0
)

env_target_link(boost_dep INTERFACE hana)

env_target_precompile(
        boost_dep
        INTERFACE
        <boost/hana.hpp>
)
