# node headers fetch

if(NOT DEFINED fwa_node_headers_download_url)
    set(fwa_node_headers_download_url [[https://nodejs.org/dist/v14.15.3/node-v14.15.3-headers.tar.gz]])
endif()

fwa_log("Node headers url: ${fwa_node_headers_download_url}...")

fwa_fetch(
        node_headers
        URL ${fwa_node_headers_download_url})

fwa_get_fetch_properties(node_headers SOURCE_DIR fwa_node_headers_download_dir)


# node lib fetch

if(NOT DEFINED fwa_node_lib_download_url)
    set(fwa_node_lib_download_url [[https://nodejs.org/dist/v14.15.3/win-x64/node.lib]])
endif()

fwa_log("Node library url: ${fwa_node_lib_download_url}...")

fwa_fetch(
        node_lib
        URL ${fwa_node_lib_download_url}
        DOWNLOAD_NO_EXTRACT TRUE)

fwa_get_fetch_properties(node_lib SOURCE_DIR fwa_node_lib_download_dir)


# node api fetch

fwa_fetch(
        node_api
        GIT_REPOSITORY https://github.com/nodejs/node-addon-api.git
        GIT_TAG 3.1.0)

fwa_get_fetch_properties(node_api SOURCE_DIR fwa_node_api_download_dir)


# dependency

fwa_add_dependency(node_api INTERFACE)
fwa_target_include(node_api INTERFACE ${fwa_node_headers_download_dir}/include/node  ${fwa_node_api_download_dir})
fwa_target_link(node_api INTERFACE ${fwa_node_lib_download_dir}/node.lib)
fwa_target_precompile(node_api INTERFACE <napi.h>)
