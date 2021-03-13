fwa_fetch(json
        GIT_REPOSITORY https://github.com/nlohmann/json.git
        GIT_TAG v3.7.3
        DOWNLOAD_DIR ${DOWNLOAD_DIRECTORY})

fwa_add_dependency(json INTERFACE)
fwa_target_link(json INTERFACE nlohmann_json::nlohmann_json)
fwa_target_precompile(json INTERFACE <nlohmann/json.hpp>)
