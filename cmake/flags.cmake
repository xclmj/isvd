function(ISVD_FLAGS_FN)
  # Create flags

  set(incs "-I${CMAKE_INSTALL_PREFIX}/include")
  foreach(i ${INCS})
    string(REGEX REPLACE "^ " "" i "${i}")
    set(incs "${incs} -isystem ${i}")
  endforeach()
  string(REGEX REPLACE "  +" " " incs "${incs}")
  string(REGEX REPLACE "^ " "" incs "${incs}")

  set(libs "")
  foreach(l ${LIBS})
    string(REGEX REPLACE "^ " "" l "${l}")
    string(SUBSTRING ${l} 0 1 l0)
    if(${l0} MATCHES "-")
      set(libs "${libs} ${l}")
    else()
      set(libs "${libs} -l${l}")
    endif()
  endforeach()
  string(REGEX REPLACE "  +" " " libs "${libs}")
  string(REGEX REPLACE "^ " "" libs "${libs}")

  set(defs "")
  foreach(d ${DEFS})
    string(REGEX REPLACE "^ " "" d "${d}")
    string(SUBSTRING ${d} 0 1 d0)
    if(${d0} MATCHES "-")
      set(defs "${defs} ${d}")
    else()
      set(defs "${defs} -D${d}")
    endif()
  endforeach()
  string(REGEX REPLACE "  +" " " cflgs "${CMAKE_C_FLAGS} -DNDEBUG ${COMFLGS} ${defs}")
  string(REGEX REPLACE "^ " "" cflgs "${cflgs}")

  string(REGEX REPLACE "  +" " " cxxflgs "${CMAKE_CXX_FLAGS} -DNDEBUG ${COMFLGS} ${defs}")
  string(REGEX REPLACE "^ " "" cxxflgs "${cxxflgs}")

  string(REGEX REPLACE "  +" " " lnkflgs "${LNKFLGS}")
  string(REGEX REPLACE "^ " "" lnkflgs "${lnkflgs}")

  # Set flags
  set(ISVD_INCS "${incs}" PARENT_SCOPE)
  set(ISVD_LIBS "${libs}" PARENT_SCOPE)
  set(ISVD_C_FLAGS "${cflgs}" PARENT_SCOPE)
  set(ISVD_CXX_FLAGS "${cxxflgs}" PARENT_SCOPE)
  set(ISVD_LNKFLGS "${lnkflgs}" PARENT_SCOPE)
endfunction()

isvd_flags_fn()
unset(isvd_flags_fn)
