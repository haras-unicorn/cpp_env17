fwa_fetch(asio URL https://www.steinberg.net/asiosdk)
fwa_get_fetch_properties(asio SOURCE_DIR fwa_asio_fetch_source_dir)

fwa_log("Asio sources in: \"${fwa_asio_fetch_source_dir}\"")

fwa_add_dependency(asio INTERFACE)
fwa_target_include(
        asio
        INTERFACE
        ${fwa_asio_fetch_source_dir}/common
        ${fwa_asio_fetch_source_dir}/host
        ${fwa_asio_fetch_source_dir}/host/pc)
