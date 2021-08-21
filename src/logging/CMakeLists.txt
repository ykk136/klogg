add_library(logging STATIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src/logger.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/log.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/logger.h
)

target_include_directories(logging PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_compile_definitions(logging PUBLIC "-DQT_MESSAGELOGCONTEXT")
target_link_libraries(logging PUBLIC 
    project_options
    project_warnings
    Qt5::Core
)