# node headers fetch

if(NOT DEFINED env_node_headers_download_url)
    set(env_node_headers_download_url [[https://nodejs.org/dist/v14.15.3/node-v14.15.3-headers.tar.gz]])
endif()

env_log("Node headers url: ${env_node_headers_download_url}...")

env_fetch(
        node_headers
        URL ${env_node_headers_download_url})

env_get_fetch_properties(node_headers SOURCE_DIR env_node_headers_download_dir)


# node lib fetch

if(NOT DEFINED env_node_lib_download_url)
    set(env_node_lib_download_url [[https://nodejs.org/dist/v14.15.3/win-x64/node.lib]])
endif()

env_log("Node library url: ${env_node_lib_download_url}...")

env_fetch(
        node_lib
        URL ${env_node_lib_download_url}
        DOWNLOAD_NO_EXTRACT TRUE)

env_get_fetch_properties(node_lib SOURCE_DIR env_node_lib_download_dir)


# node api fetch

env_fetch(
        node_api
        GIT_REPOSITORY https://github.com/nodejs/node-addon-api.git
        GIT_TAG 3.1.0)

env_get_fetch_properties(node_api SOURCE_DIR env_node_api_download_dir)


# dependency

env_add_dependency(node_api INTERFACE)
env_target_include(node_api INTERFACE ${env_node_headers_download_dir}/include/node  ${env_node_api_download_dir})
env_target_link(node_api INTERFACE ${env_node_lib_download_dir}/node.lib)
env_target_precompile(node_api INTERFACE <napi.h>)
