# The CMake setting of 'doxygen/'

isvd_set_config_var()

# Break
if(NOT ISVD_BUILD_DOC)

  # Add rule 'doc'
  add_custom_target(
    doc
    COMMAND eval "exit 1"
    COMMENT "${Esc}[1;33mWarning: Documentation are not enabled! Please set 'ISVD_BUILD_DOC' using ccmake${Esc}[0m."
  )

  return()
endif()

# Set documentation directory
set(ISVD_DOCS_DIR "${PROJECT_BINARY_DIR}/docs")
set(ISVD_DOCS_DIR "${ISVD_DOCS_DIR}" PARENT_SCOPE)

# Configure files
isvd_configure_fn("${CMAKE_CURRENT_SOURCE_DIR}" "${CMAKE_CURRENT_CONFIG_DIR}")

# Set install rule
install(DIRECTORY "${ISVD_DOCS_DIR}" DESTINATION docs OPTIONAL)

# Add rule
add_custom_target(
  doc
  COMMAND doxygen "${CMAKE_CURRENT_CONFIG_DIR}/Doxyfile"
  COMMENT "Generating documentation with Doxygen"
)

# Add to clean rule
set_property(DIRECTORY APPEND PROPERTY ADDITIONAL_MAKE_CLEAN_FILES "${ISVD_DOCS_DIR}")
