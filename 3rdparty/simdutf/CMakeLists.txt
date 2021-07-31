add_library(simdutf STATIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/simdutf.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/simdutf/simdutf.h)

target_include_directories(simdutf SYSTEM PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_include_directories(simdutf SYSTEM PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include/simdutf)
