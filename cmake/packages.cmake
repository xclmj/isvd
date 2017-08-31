if(ISVD_BUILD_BIN)
  set(findtype REQUIRED)
else()
  set(findtype "")
endif()

# Check compiler support
if(ISVD_BUILD_BIN)
  unset(cflags)
  unset(cxxflags)
  unset(fflags)

  list(APPEND cflags "-std=c99" "-O2" "-g" "-Wall" "-Wextra" "-pedantic")
  include(CheckCCompilerFlag)
  foreach(cflag ${cflags})
    string(TOUPPER ${cflag} cflagname)
    string(REGEX REPLACE "=" "_" cflagname ${cflagname})
    string(REGEX REPLACE "\\+" "X" cflagname ${cflagname})
    CHECK_C_COMPILER_FLAG(${cflag} C_SUPPORTS_${cflagname})
    if(NOT C_SUPPORTS_${cflagname})
      message(
        FATAL_ERROR
        "The compiler ${CMAKE_C_COMPILER} does not support ${cflag}. "
        "Please use a different C compiler."
      )
    endif()
  endforeach()

  list(APPEND cxxflags "-std=c++98" "-O2" "-g" "-Wall" "-Wextra" "-pedantic")
  include(CheckCXXCompilerFlag)
  foreach(cxxflag ${cxxflags})
    string(TOUPPER ${cxxflag} cxxflagname)
    string(REGEX REPLACE "=" "_" cxxflagname ${cxxflagname})
    string(REGEX REPLACE "\\+" "X" cxxflagname ${cxxflagname})
    CHECK_CXX_COMPILER_FLAG(${cxxflag} CXX_SUPPORTS_${cxxflagname})
    if(NOT CXX_SUPPORTS_${cxxflagname})
      message(
        FATAL_ERROR
        "The compiler ${CMAKE_CXX_COMPILER} does not support ${cxxflag}. "
        "Please use a different CXX compiler."
      )
    endif()
  endforeach()

  if(ISVD_USE_FORTRAN)
    list(APPEND fflags "-std=f95" "-O2" "-g" "-Wall" "-Wextra" "-pedantic" ${FILP64})
    include(CheckFortranCompilerFlag)
    foreach(fflag ${fflags})
      string(TOUPPER ${fflag} fflagname)
      string(REGEX REPLACE "=" "_" fflagname ${fflagname})
      string(REGEX REPLACE "\\+" "X" fflagname ${fflagname})
      CHECK_Fortran_COMPILER_FLAG(${fflag} Fortran_SUPPORTS_${fflagname})
      if(NOT Fortran_SUPPORTS_${fflagname})
        message(
          FATAL_ERROR
          "The compiler ${CMAKE_Fortran_COMPILER} does not support ${fflag}. "
          "Please use a different Fortran compiler."
        )
      endif()
    endforeach()
  endif()
endif()

# Set complier flags
string(REGEX REPLACE ";" " " CMAKE_C_FLAGS "${cflags}")
string(REGEX REPLACE ";" " " CMAKE_CXX_FLAGS "${cxxflags}")
string(REGEX REPLACE ";" " " CMAKE_Fortran_FLAGS "${fflags}")
set(LNKFLGS "${LNKFLGS} -Wl,--no-as-needed")

# Add library
find_library(
  M_LIBRARY
  NAMES m
  DOC "libm"
)
if(NOT M_LIBRARY)
  set(M_LIBRARY "-lm" CACHE STRING "libm" FORCE)
endif()

find_library(
  PTHREAD_LIBRARY
  NAMES pthread
  DOC "libpthread"
)
if(NOT PTHREAD_LIBRARY)
  set(PTHREAD_LIBRARY "-lpthread" CACHE STRING "libpthread" FORCE)
endif()

mark_as_advanced(M_LIBRARY PTHREAD_LIBRARY)
list(APPEND LIBS "${M_LIBRARY}" "${PTHREAD_LIBRARY}")

# MPI
find_package(MPI ${findtype})
if(MPI_FOUND)
  list(APPEND INCS "${MPI_INCLUDE_PATH}")
  list(APPEND LIBS "${MPI_LIBRARIES}")
  set(COMFLGS "${COMFLGS} ${MPI_COMPILE_FLAGS}")
  set(LNKFLGS "${LNKFLGS} ${MPI_LINK_FLAGS}")
endif()

# MKL
if(ISVD_USE_MKL)
  find_package(MKL ${findtype})
  if(MKL_FOUND)
    list(APPEND INCS "${MKL_INCLUDES}")
    list(APPEND LIBS "${MKL_LIBRARIES}")
    set(COMFLGS "${COMFLGS} ${MKL_FLAGS}")
  endif()
endif()

# LAPACK
if(NOT ISVD_USE_MKL)
  find_package(LAPACK ${findtype})
  if(LAPACK_FOUND)
    list(APPEND LIBS "${LAPACK_LIBRARIES}")
  endif()
endif()

# CUDA & MAGMA
if(ISVD_USE_GPU)
  if(NOT CUDA_TOOLKIT_ROOT_DIR)
    if(DEFINED ENV{CUDA_TOOLKIT_ROOT_DIR})
      set(CUDA_TOOLKIT_ROOT_DIR "$ENV{CUDA_TOOLKIT_ROOT_DIR}")
    elseif(DEFINED ENV{CUDADIR})
      set(CUDA_TOOLKIT_ROOT_DIR "$ENV{CUDADIR}")
    endif()
  endif()
  set(CUDA_TOOLKIT_ROOT_DIR "${CUDA_TOOLKIT_ROOT_DIR}" CACHE PATH "The root path of CUDA toolkit." FORCE)
  unset(CUDA_TOOLKIT_ROOT_DIR)

  find_package(CUDA ${findtype})
  find_package(MAGMA ${findtype})
  if(MAGMA_FOUND)
    list(APPEND INCS "${MAGMA_INCLUDES}")
    list(APPEND LIBS "${MAGMA_SPARSE_LIBRARY}" "${MAGMA_LIBRARY}")
  endif()
  if(CUDA_FOUND)
    list(APPEND INCS "${CUDA_INCLUDE_DIRS}")
    list(APPEND LIBS "${CUDA_cusparse_LIBRARY}" "${CUDA_cublas_LIBRARY}" "${CUDA_CUDART_LIBRARY}")
  endif()
endif()

# OpenMP
if(ISVD_OMP)
  set(OpenMP ${ISVD_OMP})

  find_package(OpenMP ${findtype})
  if(OpenMP_FOUND)
    set(COMFLGS "${COMFLGS} ${OpenMP_C_FLAGS}")
  endif()

  find_package(OpenMPLib ${findtype})
  if(OpenMPLib_FOUND)
    list(APPEND LIBS "${OpenMP_LIBRARIES}")
  endif()

  unset(OpenMP)
elseif(ISVD_USE_GPU)
  set(OpenMP "GOMP")

  find_package(OpenMP ${findtype})
  find_package(OpenMPLib ${findtype})
  if(OpenMPLib_FOUND)
    list(APPEND LIBS "${OpenMP_LIBRARIES}")
  endif()

  unset(OpenMP)
endif()

# GTest
if(ISVD_BUILD_TEST)
  find_package(GTest 1.8 REQUIRED)
endif()

# DOxygen
if(ISVD_BUILD_DOC)
  find_package(Doxygen REQUIRED)
endif()
