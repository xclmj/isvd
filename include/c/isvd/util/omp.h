////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file    include/c/isvd/util/omp.h
/// \brief   The OpenMP utilities.
///
/// \author  Mu Yang <<emfomy@gmail.com>>
///

#ifndef _ISVD_UTIL_OMP_H_
#define _ISVD_UTIL_OMP_H_

#include <isvd/def.h>

#if defined(__cplusplus)
extern "C" {
#endif  // __cplusplus

#if defined(ISVD_USE_OMP)
#define ISVD_OMP_PARALLEL _Pragma("omp parallel")
#else  // ISVD_USE_OMP
#define ISVD_OMP_PARALLEL
#endif  // ISVD_USE_OMP

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \ingroup  util_module
/// \brief  Returns the number of threads in the current team.
///
/// \return  The number of threads in the current team.
///
static inline omp_int_t isvd_getOmpSize( void ) {
#if defined(ISVD_USE_OMP)
  return omp_get_max_threads();
#else  // ISVD_USE_OMP
  return 1;
#endif  // ISVD_USE_OMP
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \ingroup  util_module
/// \brief  Returns a unique thread identification number within the current team.
///
/// \return  The thread identification number within the current team.
///
static inline omp_int_t isvd_getOmpRank( void ) {
#if defined(ISVD_USE_OMP)
  return omp_get_thread_num();
#else  // ISVD_USE_OMP
  return 0;
#endif  // ISVD_USE_OMP
}

#if defined(__cplusplus)
}
#endif  // __cplusplus

#endif  // _ISVD_UTIL_OMP_H_
