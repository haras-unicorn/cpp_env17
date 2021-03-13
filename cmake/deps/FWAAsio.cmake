env_fetch(asio URL https://www.steinberg.net/asiosdk)
env_get_fetch_properties(asio SOURCE_DIR env_asio_fetch_source_dir)

env_log("Asio sources in: \"${env_asio_fetch_source_dir}\"")

env_add_dependency(asio INTERFACE)
env_target_include(
        asio
        INTERFACE
        ${env_asio_fetch_source_dir}/common
        ${env_asio_fetch_source_dir}/host
        ${env_asio_fetch_source_dir}/host/pc)
