////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file       src/libisvd/core/stage/@x@_sketch.h
/// \brief      The Sketching utilities (@xname@ precision)
///
/// \author     Mu Yang <<emfomy@gmail.com>>
/// \copyright  MIT License
///

#ifndef _LIBISVD_CORE_STAGE_@X@_SKETCH_H_
#define _LIBISVD_CORE_STAGE_@X@_SKETCH_H_

#include <isvd/core/@x@_stage.h>
#include <libisvd/def.h>
#include <isvd/la.h>
#include <libisvd/util/memory.h>
#include <libisvd/util/omp.h>

#if !defined(DOXYGEN_SHOULD_SKIP_THIS)
static void sketchBlockCol(
    const isvd_Param  param,
    const char        ordera,
    const @xtype@    *a,
    const isvd_int_t  lda,
          @xtype@    *yst,
    const isvd_int_t  ldyst,
    const isvd_int_t  seed,
    const mpi_int_t   mpi_root
) {

  ISVD_UNUSED(ldyst);

  // ====================================================================================================================== //
  // Get parameters

  const isvd_int_t m   = param.nrow;
  const isvd_int_t mb  = param.nrow_each;
  const isvd_int_t Pmb = param.nrow_total;
  const isvd_int_t nj  = param.ncol_proc;
  const isvd_int_t nb  = param.ncol_each;
  const isvd_int_t Nl  = param.dim_sketch_total;

  // ====================================================================================================================== //
  // Check arguments

  switch ( ordera ) {
    case 'C': isvd_assert_ge(lda, m);  break;
    case 'R': isvd_assert_ge(lda, nj); break;
    default:  isvd_assert_fail();
  }
  isvd_assert_eq(ldyst, Nl);

  // ====================================================================================================================== //
  // Allocate memory

  @xtype@ *omegat = isvd_@x@malloc(Nl * nj);
  isvd_int_t ldomegat = Nl;

  @xtype@ *yst_ = isvd_@x@malloc(Nl * Pmb);
  isvd_int_t ldyst_ = Nl;

  // ====================================================================================================================== //
  // Random generate

  isvd_int_t seed_ = seed;
  MPI_Bcast(&seed_, sizeof(seed_), MPI_BYTE, mpi_root, param.mpi_comm);


  ISVD_OMP_PARALLEL
  {
    omp_int_t omp_size = isvd_getOmpSize();
    omp_int_t omp_rank = isvd_getOmpRank();

    isvd_int_t len   = nj * Nl / omp_size;
    isvd_int_t start = len * omp_rank;
    if ( omp_rank == omp_size-1 ) {
      len = nj * Nl - start;
    }

    isvd_VSLStreamStatePtr stream;
    isvd_vslNewStream(&stream, seed_);
    isvd_vslSkipAheadStream(stream, (nb * Nl * param.mpi_rank + start) * 2);
    isvd_v@x@RngGaussian(stream, len, omegat + start, 0.0, 1.0);

    isvd_vslDeleteStream(&stream);
  }

  // ====================================================================================================================== //
  // Project

  // Yi := A * Omegai (Yi' := Omegai' * A')
  char transa_ = (ordera == 'C') ? 'T' : 'N';
  isvd_@x@Gemm('N', transa_, Nl, m, nj, 1.0, omegat, ldomegat, a, lda, 0.0, yst_, ldyst_);

  // ====================================================================================================================== //
  // Rearrange

  MPI_Reduce_scatter_block(yst_, yst, mb*ldyst_, MPI_@X_TYPE@, MPI_SUM, param.mpi_comm);

  // ====================================================================================================================== //
  // Deallocate memory

  isvd_free(omegat);

}

static void sketchBlockRow(
    const isvd_Param  param,
    const char        ordera,
    const @xtype@    *a,
    const isvd_int_t  lda,
          @xtype@    *yst,
    const isvd_int_t  ldyst,
    const isvd_int_t  seed,
    const mpi_int_t   mpi_root
) {

  // ====================================================================================================================== //
  // Get parameters

  const isvd_int_t mj = param.nrow_proc;
  const isvd_int_t n  = param.ncol;
  const isvd_int_t Nl = param.dim_sketch_total;

  // ====================================================================================================================== //
  // Check arguments

  switch ( ordera ) {
    case 'C': isvd_assert_ge(lda, mj); break;
    case 'R': isvd_assert_ge(lda, n);  break;
    default:  isvd_assert_fail();
  }
  isvd_assert_ge(ldyst, Nl);

  // ====================================================================================================================== //
  // Allocate memory

  @xtype@ *omegat = isvd_@x@malloc(Nl * n);
  isvd_int_t ldomegat = Nl;

  // ====================================================================================================================== //
  // Random generate

  isvd_int_t seed_ = seed;
  MPI_Bcast(&seed_, sizeof(isvd_VSLStreamStatePtr), MPI_BYTE, mpi_root, param.mpi_comm);

  ISVD_OMP_PARALLEL
  {
    omp_int_t omp_size = isvd_getOmpSize();
    omp_int_t omp_rank = isvd_getOmpRank();

    isvd_int_t len   = n * Nl / omp_size;
    isvd_int_t start = len * omp_rank;
    if ( omp_rank == omp_size-1 ) {
      len = n * Nl - start;
    }

    isvd_VSLStreamStatePtr stream;
    isvd_vslNewStream(&stream, seed_);
    isvd_vslSkipAheadStream(stream, start * 2);
    isvd_v@x@RngGaussian(stream, len, omegat + start, 0.0, 1.0);

    isvd_vslDeleteStream(&stream);
  }

  // ====================================================================================================================== //
  // Project

  // Yi := A * Omegai (Yi' := Omegai' * A')
  char transa_ = (ordera == 'C') ? 'T' : 'N';
  isvd_@x@Gemm('N', transa_, Nl, mj, n, 1.0, omegat, ldomegat, a, lda, 0.0, yst, ldyst);

  // ====================================================================================================================== //
  // Deallocate memory

  isvd_free(omegat);

}
#endif  // DOXYGEN_SHOULD_SKIP_THIS

#endif  // _LIBISVD_CORE_STAGE_@X@_SKETCH_H_