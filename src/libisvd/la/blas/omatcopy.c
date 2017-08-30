////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file       src/libisvd/la/blas/omatcopy.c
/// \brief      The BLAS-like OMATCOPY routine.
///
/// \author     Mu Yang <<emfomy@gmail.com>>
/// \copyright  MIT License
///

#include <libisvd/la/blas/omatcopy.h>
#include <libisvd/util/arg.h>

#if !defined(ISVD_USE_MKL)

static inline float  rconjf( const float  z ) { return z; }
static inline double rconj(  const double z ) { return z; }

#define isvd_xomatcopy( trans, m, n, alpha, a, lda, b, ldb, conj ) \
  const char trans_ = isvd_arg2char("TRANS", trans, "NTRC", nullptr); \
  if ( !trans_ ) return; \
  switch ( trans_ ) { \
    case 'N': { \
      for ( isvd_int_t j = 0; j < n; ++j ) { \
        for ( isvd_int_t i = 0; i < m; ++i ) { \
          b[i+j*ldb] = alpha * a[i+j*lda]; \
        } \
      } \
      break; \
    } \
    case 'T': { \
      for ( isvd_int_t j = 0; j < n; ++j ) { \
        for ( isvd_int_t i = 0; i < m; ++i ) { \
          b[i*ldb+j] = alpha * a[i+j*lda]; \
        } \
      } \
      break; \
    } \
    case 'R': { \
      for ( isvd_int_t j = 0; j < n; ++j ) { \
        for ( isvd_int_t i = 0; i < m; ++i ) { \
          b[i+j*ldb] = alpha * conj(a[i+j*lda]); \
        } \
      } \
      break; \
    } \
    case 'C': { \
      for ( isvd_int_t j = 0; j < n; ++j ) { \
        for ( isvd_int_t i = 0; i < m; ++i ) { \
          b[i*ldb+j] = alpha * conj(a[i+j*lda]); \
        } \
      } \
      break; \
    } \
    default: { \
      isvd_assert_fail(); \
    } \
  }

void isvd_somatcopy(
    const char trans,
    const isvd_int_t m,
    const isvd_int_t n,
    const float alpha,
    const float *a,
    const isvd_int_t lda,
          float *b,
    const isvd_int_t ldb
) {
  isvd_xomatcopy(trans, m, n, alpha, a, lda, b, ldb, rconjf);
}
void isvd_domatcopy(
    const char trans,
    const isvd_int_t m,
    const isvd_int_t n,
    const double alpha,
    const double *a,
    const isvd_int_t lda,
          double *b,
    const isvd_int_t ldb
) {
  isvd_xomatcopy(trans, m, n, alpha, a, lda, b, ldb, rconj);
}
void isvd_comatcopy(
    const char trans,
    const isvd_int_t m,
    const isvd_int_t n,
    const complex float alpha,
    const complex float *a,
    const isvd_int_t lda,
          complex float *b,
    const isvd_int_t ldb
) {
  isvd_xomatcopy(trans, m, n, alpha, a, lda, b, ldb, conjf);
}
void isvd_zomatcopy(
    const char trans,
    const isvd_int_t m,
    const isvd_int_t n,
    const complex double alpha,
    const complex double *a,
    const isvd_int_t lda,
          complex double *b,
    const isvd_int_t ldb
) {
  isvd_xomatcopy(trans, m, n, alpha, a, lda, b, ldb, conj);
}

#endif  // ISVD_USE_MKL
