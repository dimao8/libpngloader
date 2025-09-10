cmake_minimum_required(VERSION 3.18)

project(
  pngloader
  VERSION 0.2.0.10
  DESCRIPTION "Lite PNG loader library"
  HOMEPAGE_URL "https://gitverse.ru/DimaO87/libpngloader"
  LANGUAGES C
)

include(CMakePackageConfigHelpers)
include(GNUInstallDirs)

set(
  SOURCES
  src/pngloader.c
)

set(
  HEADERS
  include/pngloader/pngloader.h
)

# Dependencies
# zlib
find_package(ZLIB REQUIRED)

# Library
option(BUILD_SHARED_LIBS "Build using shared libraries" ON)
add_library(${PROJECT_NAME} SHARED ${SOURCES})
target_include_directories(
  pngloader PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<BUILD_INTERFACE:${CMAKE_BINARY_DIR}>
  $<BUILD_INTERFACE:${ZLIB_INCLUDE_DIRS}>
  $<INSTALL_INTERFACE:include/pngloader>
)
set_target_properties(
  ${PROJECT_NAME} PROPERTIES
  PUBLIC_HEADER "${HEADERS}"
)
add_dependencies(
  pngloader pdf
)
target_compile_definitions(
  pngloader PUBLIC
  "-DHAVE_CONFIG_H"
)
target_link_libraries(
  pngloader PUBLIC
  ${ZLIB_LIBRARIES}
)

# Documentation
find_program(
  MAKEINFO
  makeinfo
)
if(MAKEINFO)
  add_subdirectory(doc)
else()
  message(WARNING "makeinfo not found. Skip document building.")
endif()

# Config file
set(PACKAGE ${PROJECT_NAME})
set(VERSION ${PROJECT_VERSION})
configure_file(
  ${CMAKE_SOURCE_DIR}/config.h.in
  ${CMAKE_BINARY_DIR}/config.h
  @ONLY
)

# Configuration
set(INCLUDE_INSTALL_DIR include/ CACHE PATH "Headers install directory")
set(LIB_INSTALL_DIR lib/ CACHE PATH "Libraries install directory")
set(SYSCONFIG_INSTALL_DIR etc/pngloader/ CACHE PATH "System Configuration install directory")
configure_package_config_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/cmake/pngloaderConfig.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/pngloaderConfig.cmake
  INSTALL_DESTINATION ${LIB_INSTALL_DIR}/pngloader/cmake
  PATH_VARS INCLUDE_INSTALL_DIR SYSCONFIG_INSTALL_DIR
)

write_basic_package_version_file(
  ${CMAKE_CURRENT_BINARY_DIR}/pngloaderConfigVersion.cmake
  VERSION 0.2.0
  COMPATIBILITY SameMajorVersion
)

install(
  TARGETS pngloader
  EXPORT pngloader-targets
  PUBLIC_HEADER DESTINATION include/pngloader
)

install(
  FILES
  ${CMAKE_CURRENT_BINARY_DIR}/pngloaderConfig.cmake
  ${CMAKE_CURRENT_BINARY_DIR}/pngloaderConfigVersion.cmake
  DESTINATION ${LIB_INSTALL_DIR}/cmake/pngloader
)

if(MAKEINFO)
  install(
    FILES ${CMAKE_BINARY_DIR}/doc/pngloader.pdf
    TYPE DOC
  )
endif()

# Testing
option(DISABLE_TESTING "Disable test routine" OFF)
if(NOT DISABLE_TESTING)
  add_subdirectory(tests)
else()
  message(NOTICE "Testing is disabled. ``check'' is unavailable")
endif()
