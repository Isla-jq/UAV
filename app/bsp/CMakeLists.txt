add_library(bsp_source INTERFACE)
add_library(bsp_include INTERFACE)
add_library(bsp_libs INTERFACE)

file(GLOB BSP_SOURCES_FILE CONFIGURE_DEPENDS Src/*.c)
file(GLOB MODULE_SOURCES_FILE CONFIGURE_DEPENDS module/*.c)
target_sources(bsp_source INTERFACE 
    ${BSP_SOURCES_FILE}
    ${MODULE_SOURCES_FILE})

target_include_directories(bsp_include
    INTERFACE
    Inc
)

target_link_libraries(bsp_libs
    INTERFACE
    bsp_source
    bsp_include
)
