set(DOC_SOURCE_DIR ${CMAKE_SOURCE_DIR}/doc)
set(DOC_BIN_DIR ${CMAKE_BINARY_DIR}/doc)
set(DOC_FILE_NAME pngloader)

set(
  DOC_SOURCES
  ${DOC_SOURCE_DIR}/${DOC_FILE_NAME}.texi
)

add_custom_target(
  pdf
  COMMAND ${MAKEINFO} --pdf --output=${DOC_FILE_NAME}.pdf ${DOC_SOURCES}
  DEPENDS ${DOC_SOURCES}
  BYPRODUCTS
    ${DOC_BIN_DIR}/${DOC_FILE_NAME}.pdf
    ${DOC_BIN_DIR}/${DOC_FILE_NAME}.log
    ${DOC_BIN_DIR}/${DOC_FILE_NAME}.aux
    ${DOC_BIN_DIR}/${DOC_FILE_NAME}.toc
)

add_custom_target(
  info
  COMMAND ${MAKEINFO} --output=${DOC_FILE_NAME}.info ${DOC_SOURCES}
  DEPENDS ${DOC_SOURCES}
  BYPRODUCTS
    ${DOC_BIN_DIR}/${DOC_FILE_NAME}.info
)

add_custom_target(
  html
  COMMAND ${MAKEINFO} --html --output=${DOC_FILE_NAME}.html ${DOC_SOURCES}
  DEPENDS ${DOC_SOURCES}
)
