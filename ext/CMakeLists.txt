# The CMake setting of 'ext/'

# Macro
macro(SET_TARGET target files)
  # Set library
  add_library(${target} EXCLUDE_FROM_ALL ${files})
  target_compile_definitions(${target} PRIVATE "NDEBUG")
  set_target_properties(${target} PROPERTIES COMPILE_FLAGS "-w")
endmacro()

# mmio
set_target(extmmio ${CMAKE_CURRENT_SOURCE_DIR}/mmio/mmio.c)
