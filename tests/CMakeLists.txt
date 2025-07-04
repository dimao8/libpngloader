enable_testing()

set(TESTS_SRC_DIR ${CMAKE_SOURCE_DIR}/tests)
set(TESTS_BIN_DIR ${CMAKE_BINARY_DIR}/tests)

# tpngloader8
add_executable(
  tpngloader8
  ${TESTS_SRC_DIR}/tpngloader8.c
)
target_include_directories(tpngloader8 PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(
  tpngloader8 PRIVATE
  pngloader
  ${ZLIB_LIBRARIES}
)

# tpngloader16
add_executable(
  tpngloader16
  ${TESTS_SRC_DIR}/tpngloader16.c
)
target_include_directories(tpngloader16 PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(
  tpngloader16 PRIVATE
  pngloader
  ${ZLIB_LIBRARIES}
)

# tpngloader24
add_executable(
  tpngloader24
  ${TESTS_SRC_DIR}/tpngloader24.c
)
target_include_directories(tpngloader24 PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(
  tpngloader24 PRIVATE
  pngloader
  ${ZLIB_LIBRARIES}
)

# tpngloader32
add_executable(
  tpngloader32
  ${TESTS_SRC_DIR}/tpngloader32.c
)
target_include_directories(tpngloader32 PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(
  tpngloader32 PRIVATE
  pngloader
  ${ZLIB_LIBRARIES}
)

# tpngloader_error
add_executable(
  tpngloader_error
  ${TESTS_SRC_DIR}/tpngloader_error.c
)
target_include_directories(tpngloader_error PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(
  tpngloader_error PRIVATE
  pngloader
  ${ZLIB_LIBRARIES}
)

add_test(
  NAME t_version
  COMMAND $<TARGET_FILE:tpngloader32> --version
  WORKING_DIRECTORY ${TESTS_BIN_DIR}
)

add_test(
  NAME t_load8
  COMMAND $<TARGET_FILE:tpngloader8> ${TESTS_SRC_DIR}/t_load8.png
  WORKING_DIRECTORY ${TESTS_BIN_DIR}
)

add_test(
  NAME t_load16
  COMMAND $<TARGET_FILE:tpngloader16> ${TESTS_SRC_DIR}/t_load16.png
  WORKING_DIRECTORY ${TESTS_BIN_DIR}
)

add_test(
  NAME t_load16bpc
  COMMAND $<TARGET_FILE:tpngloader32> ${TESTS_SRC_DIR}/t_load16bpc.png
  WORKING_DIRECTORY ${TESTS_BIN_DIR}
)

add_test(
  NAME t_load24
  COMMAND $<TARGET_FILE:tpngloader24> ${TESTS_SRC_DIR}/t_load24.png
  WORKING_DIRECTORY ${TESTS_BIN_DIR}
)

add_test(
  NAME t_load32
  COMMAND $<TARGET_FILE:tpngloader32> ${TESTS_SRC_DIR}/t_load32.png
  WORKING_DIRECTORY ${TESTS_BIN_DIR}
)

add_test(
  NAME t_chunk_error
  COMMAND $<TARGET_FILE:tpngloader_error> --except=3 ${TESTS_SRC_DIR}/t_chunk_error.png
  WORKING_DIRECTORY ${TESTS_BIN_DIR}
)
set_property(
  TEST t_chunk_error
  PROPERTY WILL_FAIL True
)

add_test(
  NAME t_compress_error
  COMMAND $<TARGET_FILE:tpngloader_error> --except=7 ${TESTS_SRC_DIR}/t_compress_error.png
  WORKING_DIRECTORY ${TESTS_BIN_DIR}
)
set_property(
  TEST t_compress_error
  PROPERTY WILL_FAIL True
)

add_test(
  NAME t_png_error
  COMMAND $<TARGET_FILE:tpngloader_error> --except=2 ${TESTS_SRC_DIR}/t_png_error.png
  WORKING_DIRECTORY ${TESTS_BIN_DIR}
)
set_property(
  TEST t_png_error
  PROPERTY WILL_FAIL True
)

add_test(
  NAME t_empty_error
  COMMAND $<TARGET_FILE:tpngloader_error> --except=5 ${TESTS_SRC_DIR}/t_empty_error.png
  WORKING_DIRECTORY ${TESTS_BIN_DIR}
)
set_property(
  TEST t_empty_error
  PROPERTY WILL_FAIL True
)

add_test(
  NAME t_file_error
  COMMAND $<TARGET_FILE:tpngloader_error> --except=1 ${TESTS_SRC_DIR}/111.png
  WORKING_DIRECTORY ${TESTS_BIN_DIR}
)
set_property(
  TEST t_file_error
  PROPERTY WILL_FAIL True
)

add_custom_target(
  check
  COMMAND ${CMAKE_CTEST_COMMAND}
  DEPENDS tpngloader32 tpngloader24 tpngloader16 tpngloader8 tpngloader_error
)
