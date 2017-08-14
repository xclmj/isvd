////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @file    demo/ctest.c
/// @brief   The C test code
///
/// @author  Mu Yang <<emfomy@gmail.com>>
///

#include <stdio.h>
#include <stdlib.h>
#include <isvd.h>

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Main function
///
int main( int argc, char **argv ) {

  MPI_Init(&argc, &argv);

  mpi_int_t mpi_size = isvd_getMpiSize(MPI_COMM_WORLD);
  mpi_int_t mpi_rank = isvd_getMpiRank(MPI_COMM_WORLD);
  mpi_int_t mpi_root = 0;

  isvd_int_t omp_size = isvd_getOmpSize();

  if ( mpi_rank == mpi_root ) {
    printf("iSVD " ISVD_VERSION " C test\n");
    printf("%d nodes, %d threads per node\n", mpi_size, omp_size);
  }

  const isvd_Param param = isvd_createParam(11, 13, 2, 1, 1, 0, MPI_COMM_WORLD);
  const isvd_int_t seed = 0;

  isvd_int_t m   = param.nrow;
  isvd_int_t mj  = param.nrow_proc;
  isvd_int_t mb  = param.nrow_each;
  isvd_int_t n   = param.ncol;
  isvd_int_t nj  = param.ncol_proc;
  isvd_int_t nb  = param.ncol_each;
  isvd_int_t Nl  = param.dim_sketch_total;

  double *a = isvd_dmalloc(m * n);
  isvd_int_t lda = m;
  for ( isvd_int_t i = 0; i < m * n; ++i ) {
    a[i] = i;
  }

  double *yt = isvd_dmalloc(mb * Nl);
  isvd_int_t ldyt = Nl;

  isvd_dSketchGaussianProjection('C', 'C', param, a + m * nb * param.mpi_rank, lda, yt, ldyt, seed, mpi_root);
  // isvd_dSketchGaussianProjection('R', 'C', param, a + mb * param.mpi_rank, lda, yt, ldyt, seed, mpi_root);

  isvd_mrdisp("%lf", mb, Nl, Nl, yt);

  MPI_Finalize();

  return 0;
}
