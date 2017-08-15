////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @file    src/libisvd/core/isvd_d_orthogonalize_gramian.c
/// @brief   The Gramian Orthogonalization (double precision)
///
/// @author  Mu Yang <<emfomy@gmail.com>>
///

#include <isvd/core/dtype.h>
#include <isvd/util/memory.h>

typedef double isvd_val_t;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @ingroup  core_dtype_module
/// Gramian Orthogonalization (double precision)
///
/// @param[in]   param       The @ref isvd_Param "parameters".
/// @param[in]   yst, ldyst  The row-block 𝖄 (@f$ m_b \times Nl @f$, row-major) and its leading dimension.
/// <hr>
/// @param[out]  yst          Replaced by the row-block 𝕼 (row-major).
///
/// @see  isvd_Param
///
void isvd_dOrthogonalizeGramian(
    const isvd_Param param,
          isvd_val_t *yst,
    const isvd_int_t ldyst
) {

  // ====================================================================================================================== //
  // Get parameters

  isvd_int_t mj = param.nrow_proc;
  isvd_int_t l  = param.dim_sketch;
  isvd_int_t N  = param.num_sketch;
  isvd_int_t Nl = param.dim_sketch_total;

  // ====================================================================================================================== //
  // Check arguments

  isvd_assert_ge(ldyst, Nl);

  // ====================================================================================================================== //
  // Allocate memory

  isvd_val_t *yst_ = isvd_dmalloc(mj * ldyst);
  isvd_int_t ldyst_ = ldyst;

  isvd_val_t *w = isvd_dmalloc(l * Nl);
  isvd_int_t ldw = l;

  isvd_val_t *s = isvd_dmalloc(l * N);
  isvd_int_t lds = l;

  isvd_val_t *superb = isvd_dmalloc(l-2);

  // ====================================================================================================================== //
  // Orthogonalize

  // Wi := Yi' * Yi
  for ( isvd_int_t i = 0; i < N; ++i ) {
    cblas_dgemm(CblasColMajor, CblasNoTrans, CblasTrans, l, l, mj,
                1.0, yst + i*l, ldyst, yst + i*l, ldyst, 0.0, w + i*ldw*l, ldw);
  }
  MPI_Allreduce(MPI_IN_PLACE, w, ldw*Nl, MPI_DOUBLE, MPI_SUM, param.mpi_comm);

  // eig(Wi) = Wi * Si * Wi'
  for ( isvd_int_t i = 0; i < N; ++i ) {
    isvd_assert_pass(LAPACKE_dgesvd(LAPACK_COL_MAJOR, 'O', 'N', l, l, w + i*ldw*l, ldw, s + i*lds, NULL, 1, NULL, 1, superb));
  }

  // Qi := Yi * Wi / sqrt(Si) (Qi' := (Wi / sqrt(Si))' * Yi' )
  vdSqrt(lds*N, s, s);
  for ( isvd_int_t ii = 0; ii < Nl; ++ii ) {
    cblas_dscal(l, 1.0/s[ii], w + ii*ldw, 1);
  }
  isvd_dmemcpy(yst_, yst, mj*ldyst);
  for ( isvd_int_t i = 0; i < N; ++i ) {
    cblas_dgemm(CblasColMajor, CblasTrans, CblasNoTrans, l, mj, l,
                1.0, w + i*ldw*l, ldw, yst_ + i*l, ldyst_, 0.0, yst + i*l, ldyst);
  }

  // ====================================================================================================================== //
  // Deallocate memory

  isvd_free(yst_);
  isvd_free(w);
  isvd_free(s);
  isvd_free(superb);

}
