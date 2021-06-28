add_library(settings STATIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src/persistentinfo.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/configuration.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/shortcuts.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/styles.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/persistable.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/persistentinfo.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/configuration.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/styles.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/shortcuts.h
)

target_include_directories(settings PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_link_libraries(settings PUBLIC 
    project_options
    project_warnings
    utils
    Qt5::Core
    Qt5::Gui
)