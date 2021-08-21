add_library(utils STATIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include/atomicflag.h
        ${CMAKE_CURRENT_SOURCE_DIR}/include/uuid.h
        ${CMAKE_CURRENT_SOURCE_DIR}/include/dispatch_to.h
        ${CMAKE_CURRENT_SOURCE_DIR}/include/perfcounter.h
        ${CMAKE_CURRENT_SOURCE_DIR}/include/openfilehelper.h
        ${CMAKE_CURRENT_SOURCE_DIR}/include/progress.h
        ${CMAKE_CURRENT_SOURCE_DIR}/include/synchronization.h
        ${CMAKE_CURRENT_SOURCE_DIR}/include/overload_visitor.h
        ${CMAKE_CURRENT_SOURCE_DIR}/include/resourcewrapper.h
        ${CMAKE_CURRENT_SOURCE_DIR}/include/crc32.h)

set_target_properties(utils PROPERTIES AUTOMOC ON)

target_include_directories(utils PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_link_libraries(utils PUBLIC
        project_options
        project_warnings
        klogg_version
        logging
        Qt5::Gui
        Qt5::Widgets
        Qt5::Network
        Qt5::Concurrent
        whereami)


