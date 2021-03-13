env_fetch(json
        GIT_REPOSITORY https://github.com/nlohmann/json.git
        GIT_TAG v3.7.3
        DOWNLOAD_DIR ${DOWNLOAD_DIRECTORY})

env_add_dependency(json INTERFACE)
env_target_link(json INTERFACE nlohmann_json::nlohmann_json)
env_target_precompile(json INTERFACE <nlohmann/json.hpp>)
