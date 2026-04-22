add_library(bsp_source INTERFACE)
add_library(bsp_include INTERFACE)
add_library(bsp_lib INTERFACE)

target_sources(bsp_source
    INTERFACE
    Src/bsp_stream_buffer_interface.c)

target_include_directories(bsp_include
    INTERFACE
    Inc)

target_link_libraries(bsp_lib
    INTERFACE
    bsp_source
    bsp_include)