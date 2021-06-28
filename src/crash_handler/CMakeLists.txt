add_library(crash_handler STATIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include/crashhandler.h
        ${CMAKE_CURRENT_SOURCE_DIR}/include/issuereporter.h
        ${CMAKE_CURRENT_SOURCE_DIR}/include/memory_info.h
        ${CMAKE_CURRENT_SOURCE_DIR}/include/cpu_info.h
        ${CMAKE_CURRENT_SOURCE_DIR}/src/issuereporter.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/memory_info.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/cpu_info.cpp
)

set_target_properties(crash_handler PROPERTIES AUTOMOC ON)

target_include_directories(crash_handler PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_link_libraries(crash_handler PUBLIC 
    project_options
    project_warnings
    klogg_version
    utils
    Qt5::Gui
    Qt5::Widgets
)

if(KLOGG_USE_SENTRY)
    target_compile_definitions(crash_handler PUBLIC -DKLOGG_USE_SENTRY)
    target_sources(crash_handler PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src/crashhandler.cpp)
    target_link_libraries(crash_handler PUBLIC sentry crashpad_client)
endif()