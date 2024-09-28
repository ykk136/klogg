set(QAPPLICATION_CLASS QApplication)

include(CPM)

function(Add3rdPartyPackage)
  cpm_set_policies()

  # ======== Parse args provided by caller ========
  message("")
  message(STATUS "init ARGN:${ARGN}, ARGC:${ARGC}, ARGV:${ARGV}, ARGV0:${ARGV0} ----")

  set(oneValueArgs
      NAME
      EXCLUDE_FROM_ALL
      SYSTEM
      ONLY_CHECK_EXIST
  )
  set(multiValueArgs OPTIONS)

  cmake_parse_arguments(CPM_ARGS "" "${oneValueArgs}" "${multiValueArgs}" "${ARGN}")

  if(NOT DEFINED CPM_ARGS_EXCLUDE_FROM_ALL)
    set(CPM_ARGS_EXCLUDE_FROM_ALL 1)
  endif()

  if(NOT DEFINED CPM_ARGS_SYSTEM)
    set(CPM_ARGS_SYSTEM 1)
  endif()

  if(DEFINED CPM_ARGS_OPTIONS)
    foreach(OPTION ${CPM_ARGS_OPTIONS})
      cpm_parse_option("${OPTION}")
      message(STATUS "set ${OPTION_KEY} to ${OPTION_VALUE} for ${CPM_ARGS_NAME}")
      set(${OPTION_KEY} "${OPTION_VALUE}")
    endforeach()
  else()
    message(STATUS "No options defined for ${CPM_ARGS_NAME}")
  endif()
  # ======== Parse args provided by caller ========

  message(STATUS "Adding subdirectory for name:${CPM_ARGS_NAME}")
  set(PKG_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/${CPM_ARGS_NAME} CACHE INTERNAL "")
  set(PKG_BIN_DIR ${CMAKE_BINARY_DIR}/${CPM_ARGS_NAME} CACHE INTERNAL "")
  if(EXISTS ${PKG_SRC_DIR})
    set(addSubdirectoryExtraArgs "")

    if(${CPM_ARGS_EXCLUDE_FROM_ALL})
      list(APPEND addSubdirectoryExtraArgs EXCLUDE_FROM_ALL)
      message(STATUS "Append EXCLUDE_FROM_ALL to the addSubdirectoryExtraArgs")
    endif()

    if(${CPM_ARGS_SYSTEM} AND ${CMAKE_VERSION} VERSION_GREATER_EQUAL "3.25")
      # https://cmake.org/cmake/help/latest/prop_dir/SYSTEM.html#prop_dir:SYSTEM
      list(APPEND addSubdirectoryExtraArgs SYSTEM)
      message(STATUS "Append SYSTEM to the addSubdirectoryExtraArgs")
    endif()

    if(${CPM_ARGS_ONLY_CHECK_EXIST})
      message(STATUS "Only check src dir exist, need handle it in parent scope")
    elseif(NOT EXISTS ${PKG_SRC_DIR}/CMakeLists.txt)
      # Bypass this error because some projects' CMakeLists.txt file is not under top directory
      # So this message will print warning log
      message(WARNING "No CMakeLists.txt found under:${PKG_SRC_DIR}")
    else()
      add_subdirectory(${PKG_SRC_DIR} ${PKG_BIN_DIR} ${addSubdirectoryExtraArgs})
    endif()

    set(${CPM_ARGS_NAME}_ADDED ON PARENT_SCOPE)
    message(STATUS "${PKG_SRC_DIR} is added")
  else()
    message(FATAL_ERROR "${PKG_SRC_DIR} doesn't exist!")
  endif()
endfunction(Add3rdPartyPackage)


# Package: simdutf
Add3rdPartyPackage(NAME simdutf)

# Package: type_safe
Add3rdPartyPackage(NAME type_safe)

# Package: arl/macdeployqtfix for APPLE platform
if(APPLE)
  Add3rdPartyPackage(
    NAME macdeployqtfix
    ONLY_CHECK_EXIST ON
  )
  if(macdeployqtfix_ADDED)
    configure_file(${macdeployqtfix_SOURCE_DIR}/macdeployqtfix.py
        ${CMAKE_BINARY_DIR}/macdeployqtfix.py COPYONLY)
  endif()
endif()

# Package: CRoaring
Add3rdPartyPackage(
  NAME CRoaring
  OPTIONS "ENABLE_ROARING_TESTS OFF"
)

# Package: maddy
Add3rdPartyPackage(NAME maddy)

# Package: hyperscan
if(KLOGG_USE_HYPERSCAN)
  Add3rdPartyPackage(NAME hyperscan)
  if(hyperscan_ADDED)
    message("Adding alias for hyperscan")
    add_library(hyperscan_wrapper INTERFACE)
    target_link_libraries(hyperscan_wrapper INTERFACE hs)
    target_include_directories(hyperscan_wrapper INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/hyperscan/src)
  else()
    message(ERROR "Can't find hyperscan")
    add_library(hyperscan_wrapper INTERFACE)
    target_link_libraries(hyperscan_wrapper INTERFACE ${HYPERSCAN_LIBRARY})
    target_include_directories(hyperscan_wrapper INTERFACE ${HYPERSCAN_INCLUDE_DIR})
  endif()
elseif(KLOGG_USE_VECTORSCAN)
  Add3rdPartyPackage(NAME vectorscan)
  if(vectorscan_ADDED)
    message("Adding alias for vectorscan")
    add_library(vectorscan_wrapper INTERFACE)
    target_link_libraries(vectorscan_wrapper INTERFACE hs)
    target_include_directories(vectorscan_wrapper INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/vectorscan/src)
  else()
    message(ERROR "Can't find hyperscan")
    add_library(vectorscan_wrapper INTERFACE)
    target_link_libraries(vectorscan_wrapper INTERFACE ${VECTORSCAN_LIBRARY})
    target_include_directories(vectorscan_wrapper INTERFACE ${VECTORSCAN_INCLUDE_DIR})
  endif()
endif()

# Package: uchardet
Add3rdPartyPackage(
  NAME uchardet
  OPTIONS "BUILD_BINARY OFF"
)
if(uchardet_ADDED)
  message("Adding alias for uchardet")
  add_library(klogg_uchardet_wrapper INTERFACE)
  target_link_libraries(klogg_uchardet_wrapper INTERFACE libuchardet)
  target_include_directories(klogg_uchardet_wrapper INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/uchardet/src)
else()
  message(ERROR "Can't find uchardet")
  add_library(klogg_uchardet_wrapper INTERFACE)
  target_link_libraries(klogg_uchardet_wrapper INTERFACE ${UCHARDET_LIBRARY})
  target_include_directories(klogg_uchardet_wrapper INTERFACE ${UCHARDET_INCLUDE_DIR})
endif()

# Package: klogg_karchive
Add3rdPartyPackage(NAME klogg_karchive)

# Package: robin-hood-hashing
Add3rdPartyPackage(NAME robin-hood-hashing)

# Package: backward-cpp, Catch2
if(KLOGG_BUILD_TESTS)
  Add3rdPartyPackage(NAME backward-cpp)
  Add3rdPartyPackage(NAME Catch2)
endif()

# Package: KDSingleApplication
Add3rdPartyPackage(
  NAME KDSingleApplication
  ONLY_CHECK_EXIST ON
)
if(KDSingleApplication_ADDED)
  add_library(
    kdsingleapp STATIC ${CMAKE_CURRENT_SOURCE_DIR}/KDSingleApplication/src/kdsingleapplication.cpp
                       ${CMAKE_CURRENT_SOURCE_DIR}/KDSingleApplication/src/kdsingleapplication_localsocket.cpp
  )

  target_include_directories(kdsingleapp PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/KDSingleApplication/src)
  target_link_libraries(kdsingleapp Qt${QT_VERSION_MAJOR}::Core)
  target_compile_definitions(kdsingleapp PUBLIC KDSINGLEAPPLICATION_STATIC_BUILD)

  set_target_properties(kdsingleapp PROPERTIES AUTOMOC ON)
endif()

# Package: xxHash
Add3rdPartyPackage(
  NAME xxHash
  ONLY_CHECK_EXIST ON
)
if(xxHash_ADDED)
  set(XXHASH_BUILD_ENABLE_INLINE_API OFF)
  set(XXHASH_BUILD_XXHSUM OFF)
  add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/xxHash/cmake_unofficial ${CMAKE_BINARY_DIR}/xxhash EXCLUDE_FROM_ALL)
endif()

# Package: whereami
Add3rdPartyPackage(
  NAME whereami
  ONLY_CHECK_EXIST ON
)
if(whereami_ADDED)
  add_library(whereami STATIC
      ${CMAKE_CURRENT_SOURCE_DIR}/whereami/src/whereami.h
      ${CMAKE_CURRENT_SOURCE_DIR}/whereami/src/whereami.c)
  target_include_directories(whereami PUBLIC
      ${CMAKE_CURRENT_SOURCE_DIR}/whereami/src)
endif()

# Package: klogg_exprtk
Add3rdPartyPackage(
  NAME klogg_exprtk
  ONLY_CHECK_EXIST ON
)
if(klogg_exprtk_ADDED)
  add_library(exprtk INTERFACE)
  target_link_libraries(exprtk INTERFACE robin_hood)
  target_include_directories(exprtk SYSTEM INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/klogg_exprtk)
  target_compile_definitions(
    exprtk
    INTERFACE -Dexprtk_disable_caseinsensitivity
              -Dexprtk_disable_comments
              -Dexprtk_disable_break_continue
              -Dexprtk_disable_return_statement
              -Dexprtk_disable_superscalar_unroll
              -Dexprtk_disable_rtl_io_file
              -Dexprtk_disable_rtl_vecops
              -Dexprtk_disable_string_capabilities
  )
endif()

# Package: KDToolBox
Add3rdPartyPackage(
  NAME KDToolBox
  ONLY_CHECK_EXIST ON
)
if(KDToolBox_ADDED)
  add_library(
    kdtoolbox STATIC ${CMAKE_CURRENT_SOURCE_DIR}/KDToolBox/qt/KDSignalThrottler/src/KDSignalThrottler.cpp
                     ${CMAKE_CURRENT_SOURCE_DIR}/KDToolBox/qt/KDSignalThrottler/src/KDSignalThrottler.h
  )

  target_include_directories(kdtoolbox PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/KDToolBox/qt/KDSignalThrottler/src)
  target_link_libraries(kdtoolbox Qt${QT_VERSION_MAJOR}::Core)
  set_target_properties(kdtoolbox PROPERTIES AUTOMOC ON)
endif()

# Package: efsw
Add3rdPartyPackage(NAME efsw)
if(efsw_ADDED)
  target_compile_definitions(efsw PRIVATE EFSW_FSEVENTS_NOT_SUPPORTED)
endif()

# Package: oneTBB
if(WIN32)
  set(BUILD_SHARED_LIBS ON)
endif()
Add3rdPartyPackage(
  NAME oneTBB
  OPTIONS "TBB_TEST OFF"
          "TBB_EXAMPLES OFF"
          "TBB_STRICT OFF"
)
if(WIN32)
  set(BUILD_SHARED_LIBS OFF)
endif()

# Package: mimalloc
Add3rdPartyPackage(
  NAME mimalloc
  OPTIONS "MI_BUILD_TESTS OFF"
          "MI_SECURE OFF"
          "MI_BUILD_SHARED OFF"
          "MI_BUILD_STATIC ON"
          "MI_BUILD_OBJECT OFF"
          "MI_OVERRIDE OFF"
)
if(mimalloc_ADDED)
  message("Adding alias for mimalloc")
  add_library(klogg_mimalloc_wrapper INTERFACE)
  target_link_libraries(klogg_mimalloc_wrapper INTERFACE mimalloc-static)
  target_include_directories(klogg_mimalloc_wrapper INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/mimalloc/include)
else()
  message(ERROR "Can't find mimalloc")
  add_library(klogg_mimalloc_wrapper INTERFACE)
  target_link_libraries(klogg_mimalloc_wrapper INTERFACE ${MIMALLOC_LIBRARY})
  target_include_directories(klogg_mimalloc_wrapper INTERFACE ${MIMALLOC_INCLUDE_DIR})
endif()

# Package: sentry-native
if(KLOGG_USE_SENTRY)
  set(SENTRY_BACKEND
     "crashpad"
     CACHE INTERNAL "" FORCE
  )
  set(SENTRY_TRANSPORT
     "none"
     CACHE INTERNAL "" FORCE
  )
  set(SENTRY_BUILD_EXAMPLES
     OFF
     CACHE INTERNAL "" FORCE
  )
  set(CRASHPAD_ENABLE_INSTALL
     OFF
     CACHE INTERNAL "" FORCE
  )
  set(CRASHPAD_ENABLE_INSTALL_DEV
     OFF
     CACHE INTERNAL "" FORCE
  )

  Add3rdPartyPackage(NAME sentry-native)
  if(WIN32)
    set_target_properties(crashpad_handler PROPERTIES OUTPUT_NAME "klogg_crashpad_handler.exe")
  else()
    set_target_properties(crashpad_handler PROPERTIES OUTPUT_NAME "klogg_crashpad_handler")
  endif()
endif(KLOGG_USE_SENTRY)


set(klogg_cpm_targets
    xxhash
    Catch2
    roaring
    roaring-headers-cpp
    libuchardet
    robin_hood
    whereami
    simdutf
    efsw
    SingleApplication
    hs
    tbb
    exprtk
    type_safe
    mimalloc-static
    sentry
    crashpad_client
    crashpad_compat
    crashpad_util
    mini_chromium
)
foreach(target ${klogg_cpm_targets})
  if(TARGET ${target})
    get_property(
      ${target}_include_dirs
      TARGET ${target}
      PROPERTY INTERFACE_INCLUDE_DIRECTORIES
    )
    #message("Marking ${${target}_include_dirs} as system incldue")
    set_property(TARGET ${target} PROPERTY INTERFACE_SYSTEM_INCLUDE_DIRECTORIES ${${target}_include_dirs})
  endif()
endforeach()