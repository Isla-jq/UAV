add_library(FreeRTOS_CLI STATIC)
target_sources(FreeRTOS_CLI PUBLIC
    ./FreeRTOS_CLI.c)
target_include_directories(FreeRTOS_CLI PUBLIC
    .)