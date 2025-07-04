cmake_minimum_required(VERSION 3.15)

project(
  libpngloader
  VERSION 0.2.0.8
  DESCRIPTION "Lite PNG loader library"
  HOMEPAGE_URL "https://gitverse.ru/DimaO87/libpngloader"
  LANGUAGES C
)

set(
  SOURCES
  src/pngloader.c
)

set(
  HEADERS
  include/pngloader.h
)

# Dependencies
# zlib
find_package(ZLIB REQUIRED)

# Library
option(BUILD_SHARED_LIBS "Build using shared libraries" ON)

add_library(
  pngloader
  ${SOURCES}
  ${HEADERS}
)
add_dependencies(
  pngloader pdf
)
target_include_directories(
  pngloader PUBLIC
  ${CMAKE_SOURCE_DIR}/include
  ${CMAKE_BINARY_DIR}
  ${ZLIB_INCLUDE_DIRS}
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

# Tests
option(DISABLE_TESTING "Disable test routine" OFF)
if(NOT DISABLE_TESTING)
  add_subdirectory(tests)
else()
  message(NOTICE "Testing is disabled. ``check'' is unavailable")
endif()

# Config file
set(PACKAGE ${PROJECT_NAME})
set(VERSION ${PROJECT_VERSION})
configure_file(
  ${CMAKE_SOURCE_DIR}/config.h.in
  ${CMAKE_BINARY_DIR}/config.h
  @ONLY
)

# Copy compile_commands.json
file(
  CREATE_LINK
  ${CMAKE_BINARY_DIR}/compile_commands.json
  ${CMAKE_SOURCE_DIR}/compile_commands.json
  COPY_ON_ERROR
  SYMBOLIC
)

install(
  TARGETS pngloader
  PUBLIC_HEADER
)

if(MAKEINFO)
  install(
    FILES ${CMAKE_BINARY_DIR}/doc/pngloader.pdf
    TYPE DOC
  )
endif()