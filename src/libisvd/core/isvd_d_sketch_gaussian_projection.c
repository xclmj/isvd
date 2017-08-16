////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @file    src/libisvd/core/isvd_d_sketch_gaussian_projection.c
/// @brief   The Gaussian Projection Sketching (double precision)
///
/// @author  Mu Yang <<emfomy@gmail.com>>
///

#include <isvd/core/dtype.h>
#include <isvd/util/memory.h>
#include <isvd/util/omp.h>

typedef double isvd_val_t;

#ifndef DOXYGEN_SHOULD_SKIP_THIS
void sketchBlockCol(
    const isvd_Param param,
    const char ordera,
    const isvd_val_t *a,
    const isvd_int_t lda,
          isvd_val_t *yst,
    const isvd_int_t ldyst,
    const isvd_int_t seed,
    const mpi_int_t mpi_root
) {

  ISVD_UNUSED(ldyst);

  // ====================================================================================================================== //
  // Get parameters

  isvd_int_t m   = param.nrow;
  isvd_int_t mb  = param.nrow_each;
  isvd_int_t Pmb = param.nrow_total;
  isvd_int_t nj  = param.ncol_proc;
  isvd_int_t nb  = param.ncol_each;
  isvd_int_t Nl  = param.dim_sketch_total;

  // ====================================================================================================================== //
  // Check arguments

  switch ( ordera ) {
    case 'C': isvd_assert_ge(lda, m);  break;
    case 'R': isvd_assert_ge(lda, nj); break;
  }
  isvd_assert_eq(ldyst, Nl);

  // ====================================================================================================================== //
  // Allocate memory

  isvd_val_t *omegat = isvd_dmalloc(nj * Nl);
  isvd_int_t ldomegat = Nl;

  isvd_val_t *yst_ = isvd_dmalloc(Pmb * Nl);
  isvd_int_t ldyst_ = Nl;

  // ====================================================================================================================== //
  // Random generate

  isvd_int_t seed_ = seed;
  MPI_Bcast(&seed_, sizeof(VSLStreamStatePtr), MPI_BYTE, mpi_root, param.mpi_comm);

#ifdef _OPENMP
  #pragma omp parallel
#endif  // _OPENMP
  {
    isvd_int_t omp_size = isvd_getOmpSize();
    isvd_int_t omp_rank = isvd_getOmpRank();

    isvd_int_t len   = nj * Nl / omp_size;
    isvd_int_t start = len * omp_rank;
    if ( omp_rank == omp_size-1 ) {
      len = nj * Nl - start;
    }

    VSLStreamStatePtr stream;
    vslNewStream(&stream, VSL_BRNG_SFMT19937, seed_);
    vslSkipAheadStream(stream, (nb * Nl * param.mpi_rank + start) * 2);
    vdRngGaussian(VSL_RNG_METHOD_GAUSSIAN_BOXMULLER, stream, len, omegat + start, 0.0, 1.0);

    vslDeleteStream(&stream);
  }

  // ====================================================================================================================== //
  // Project

  // Yi := A * Omegai (Yi' := Omegai' * A')
  CBLAS_TRANSPOSE transa_ = (ordera == 'C') ? CblasTrans : CblasNoTrans;
  cblas_dgemm(CblasColMajor, CblasNoTrans, transa_, Nl, m, nj,
              1.0, omegat, ldomegat, a, lda, 0.0, yst_, ldyst_);

  // ====================================================================================================================== //
  // Rearrange

  MPI_Reduce_scatter_block(yst_, yst, mb*ldyst_, MPI_DOUBLE, MPI_SUM, param.mpi_comm);

  // ====================================================================================================================== //
  // Deallocate memory

  isvd_free(omegat);

}

void sketchBlockRow(
    const isvd_Param param,
    const char ordera,
    const isvd_val_t *a,
    const isvd_int_t lda,
          isvd_val_t *yst,
    const isvd_int_t ldyst,
    const isvd_int_t seed,
    const mpi_int_t mpi_root
) {

  // ====================================================================================================================== //
  // Get parameters

  isvd_int_t mj = param.nrow_proc;
  isvd_int_t n  = param.ncol;
  isvd_int_t Nl = param.dim_sketch_total;

  // ====================================================================================================================== //
  // Check arguments

  switch ( ordera ) {
    case 'C': isvd_assert_ge(lda, mj); break;
    case 'R': isvd_assert_ge(lda, n);  break;
  }
  isvd_assert_ge(ldyst, Nl);

  // ====================================================================================================================== //
  // Allocate memory

  isvd_val_t *omegat = isvd_dmalloc(n * Nl);
  isvd_int_t ldomegat = Nl;

  // ====================================================================================================================== //
  // Random generate

  isvd_int_t seed_ = seed;
  MPI_Bcast(&seed_, sizeof(VSLStreamStatePtr), MPI_BYTE, mpi_root, param.mpi_comm);

#ifdef _OPENMP
  #pragma omp parallel
#endif  // _OPENMP
  {
    isvd_int_t omp_size = isvd_getOmpSize();
    isvd_int_t omp_rank = isvd_getOmpRank();

    isvd_int_t len   = n * Nl / omp_size;
    isvd_int_t start = len * omp_rank;
    if ( omp_rank == omp_size-1 ) {
      len = n * Nl - start;
    }

    VSLStreamStatePtr stream;
    vslNewStream(&stream, VSL_BRNG_SFMT19937, seed_);
    vslSkipAheadStream(stream, start * 2);
    vdRngGaussian(VSL_RNG_METHOD_GAUSSIAN_BOXMULLER, stream, len, omegat + start, 0.0, 1.0);

    vslDeleteStream(&stream);
  }

  // ====================================================================================================================== //
  // Project

  // Yi := A * Omegai (Yi' := Omegai' * A')
  CBLAS_TRANSPOSE transa_ = (ordera == 'C') ? CblasTrans : CblasNoTrans;
  cblas_dgemm(CblasColMajor, CblasNoTrans, transa_, Nl, mj, n, 1.0, omegat, ldomegat, a, lda, 0.0, yst, ldyst);

  // ====================================================================================================================== //
  // Deallocate memory

  isvd_free(omegat);

}
#endif  // DOXYGEN_SHOULD_SKIP_THIS

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @ingroup  core_dtype_module
/// Gaussian Projection Sketching (double precision)
///
/// @param[in]   param        The @ref isvd_Param "parameters".
/// @param[in]   args, largs  The arguments and its length. (not using)
/// @param[in]   rets, lrets  The return values and its length. (not using)
/// <hr>
/// @param[in]   dista        The parallel distribution of 𝑨. <br>
///                           `'C'`: block-column parallelism. <br>
///                           `'R'`: block-row parallelism.
/// @param[in]   ordera       The storage ordering of 𝑨. <br>
///                           `'C'`: column-major ordering. <br>
///                           `'R'`: row-major ordering.
/// @param[in]   a, lda       The column/row-block 𝑨 (@f$m \times n_j@f$) and its leading dimension. <br>
///                           `dista='C'`: the size must be @f$m \times n_j@f$. <br>
///                           `dista='R'`: the size must be @f$m_j \times n@f$.
/// @param[in]   yst, ldyst   The row-block 𝖄 (@f$m_b \times Nl@f$, row-major) and its leading dimension. <br>
///                           `dista='C'`: @p ldyst must be @f$Nl@f$. <br>
///                           `dista='R'`: no condition.
/// @param[in]   seed         The random seed (significant only at root MPI process).
/// @param[in]   mpi_root     The root MPI process ID.
/// <hr>
/// @param[out]  yst          Replaced by the row-block 𝖄 (row-major).
///
void isvd_dSketchGaussianProjection(
    const isvd_Param param,
    const isvd_val_t *args,
    const isvd_int_t largs,
          isvd_val_t *rets,
    const isvd_int_t lrets,
    const char dista,
    const char ordera,
    const isvd_val_t *a,
    const isvd_int_t lda,
          isvd_val_t *yst,
    const isvd_int_t ldyst,
    const isvd_int_t seed,
    const mpi_int_t mpi_root
) {

  ISVD_UNUSED(args);
  ISVD_UNUSED(largs);
  ISVD_UNUSED(rets);
  ISVD_UNUSED(lrets);

  // ====================================================================================================================== //
  // Check arguments

  char dista_  = isvd_arg2char("DISTA",  dista,  "CR", "CR");
  char ordera_ = isvd_arg2char("ORDERA", ordera, "CR", "CR");
  if ( dista_ == '\0' || ordera_ == '\0' ) abort();

  // ====================================================================================================================== //
  // Run

  switch ( dista_ ) {
    case 'C': sketchBlockCol(param, ordera_, a, lda, yst, ldyst, seed, mpi_root); break;
    case 'R': sketchBlockRow(param, ordera_, a, lda, yst, ldyst, seed, mpi_root); break;
  }
}
