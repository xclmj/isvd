////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file       include/c/isvd/la/lapack/least_square.h
/// \brief      The LAPACK least square and eigenvalue problem header.
///
/// \author     Mu Yang <<emfomy@gmail.com>>
/// \copyright  MIT License
///

#ifndef ISVD_LA_LAPACK_LEAST_SQUARE_H_
#define ISVD_LA_LAPACK_LEAST_SQUARE_H_

#include <isvd/def.h>

#if defined(__cplusplus)
extern "C" {
#endif  // __cplusplus

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \ingroup  c_la_lapack_ls_module
/// \brief  Computes the QR factorization of a general matrix.
//\{
void isvd_sGeqrf( const INT m, const INT n, REAL4 *a, const INT lda, REAL4 *tau );
void isvd_dGeqrf( const INT m, const INT n, REAL8 *a, const INT lda, REAL8 *tau );
void isvd_cGeqrf( const INT m, const INT n, COMP4 *a, const INT lda, COMP4 *tau );
void isvd_zGeqrf( const INT m, const INT n, COMP8 *a, const INT lda, COMP8 *tau );
//\}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \ingroup  c_la_lapack_ls_module
/// \brief  Generates the real orthogonal matrix Q of the QR factorization formed by ?geqrf.
//\{
void isvd_sOrgqr( const INT m, const INT n, const INT k, REAL4 *a, const INT lda, REAL4 *tau );
void isvd_dOrgqr( const INT m, const INT n, const INT k, REAL8 *a, const INT lda, REAL8 *tau );
void isvd_cOrgqr( const INT m, const INT n, const INT k, COMP4 *a, const INT lda, COMP4 *tau );
void isvd_zOrgqr( const INT m, const INT n, const INT k, COMP8 *a, const INT lda, COMP8 *tau );
//\}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \ingroup  c_la_lapack_ls_module
/// \brief  Computes the LQ factorization of a general matrix.
//\{
void isvd_sGelqf( const INT m, const INT n, REAL4 *a, const INT lda, REAL4 *tau );
void isvd_dGelqf( const INT m, const INT n, REAL8 *a, const INT lda, REAL8 *tau );
void isvd_cGelqf( const INT m, const INT n, COMP4 *a, const INT lda, COMP4 *tau );
void isvd_zGelqf( const INT m, const INT n, COMP8 *a, const INT lda, COMP8 *tau );
//\}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \ingroup  c_la_lapack_ls_module
/// \brief  Generates the real orthogonal matrix Q of the LQ factorization formed by ?gelqf.
//\{
void isvd_sOrglq( const INT m, const INT n, const INT k, REAL4 *a, const INT lda, REAL4 *tau );
void isvd_dOrglq( const INT m, const INT n, const INT k, REAL8 *a, const INT lda, REAL8 *tau );
void isvd_cOrglq( const INT m, const INT n, const INT k, COMP4 *a, const INT lda, COMP4 *tau );
void isvd_zOrglq( const INT m, const INT n, const INT k, COMP8 *a, const INT lda, COMP8 *tau );
//\}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \ingroup  c_la_lapack_ls_module
/// \brief  Computes all eigenvalues and, optionally, eigenvectors of a real symmetric matrix.
//\{
void isvd_sSyev( const char jobz, const char uplo, const isvd_int_t n, isvd_s_val_t *a, const isvd_int_t lda, isvd_s_val_t *w );
void isvd_dSyev( const char jobz, const char uplo, const isvd_int_t n, isvd_d_val_t *a, const isvd_int_t lda, isvd_d_val_t *w );
void isvd_cSyev( const char jobz, const char uplo, const isvd_int_t n, isvd_c_val_t *a, const isvd_int_t lda, isvd_s_val_t *w );
void isvd_zSyev( const char jobz, const char uplo, const isvd_int_t n, isvd_z_val_t *a, const isvd_int_t lda, isvd_d_val_t *w );
//\}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \ingroup  c_la_lapack_ls_module
/// \brief  Computes the singular value decomposition of a general rectangular matrix.
//\{
void isvd_sGesvd(
    const char jobu, const char jobvt, const isvd_int_t m, const isvd_int_t n, isvd_s_val_t *a, const isvd_int_t lda,
    isvd_s_val_t *s, isvd_s_val_t *u, const isvd_int_t ldu, isvd_s_val_t *v, const isvd_int_t ldvt
);
void isvd_dGesvd(
    const char jobu, const char jobvt, const isvd_int_t m, const isvd_int_t n, isvd_d_val_t *a, const isvd_int_t lda,
    isvd_d_val_t *s, isvd_d_val_t *u, const isvd_int_t ldu, isvd_d_val_t *v, const isvd_int_t ldvt
);
void isvd_cGesvd(
    const char jobu, const char jobvt, const isvd_int_t m, const isvd_int_t n, isvd_c_val_t *a, const isvd_int_t lda,
    isvd_s_val_t *s, isvd_c_val_t *u, const isvd_int_t ldu, isvd_c_val_t *v, const isvd_int_t ldvt
);
void isvd_zGesvd(
    const char jobu, const char jobvt, const isvd_int_t m, const isvd_int_t n, isvd_z_val_t *a, const isvd_int_t lda,
    isvd_d_val_t *s, isvd_z_val_t *u, const isvd_int_t ldu, isvd_z_val_t *v, const isvd_int_t ldvt
);
//\}

#if defined(__cplusplus)
}
#endif  // __cplusplus

#endif  // ISVD_LA_LAPACK_LEAST_SQUARE_H_
