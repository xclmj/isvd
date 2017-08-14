////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @file    include/c/isvd/core/param.h
/// @brief   The parameter structure.
///
/// @author  Mu Yang <<emfomy@gmail.com>>
///

#ifndef _ISVD_CORE_PARAM_H_
#define _ISVD_CORE_PARAM_H_

#include <isvd/core/def.h>
#include <isvd/core/idx_range.h>

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @ingroup  core_param_module
/// The parameters.
///
typedef struct {

  /// The MPI communicator.
  const MPI_Comm mpi_comm;

  /// @f$P@f$ The MPI size.
  const mpi_int_t mpi_size;

  /// @f$j@f$ The MPI rank.
  const mpi_int_t mpi_rank;

  /// @f$m@f$ The number of rows of the matrix.
  const isvd_int_t nrow;

  /// @f$n@f$ The number of columns of the matrix.
  const isvd_int_t ncol;

  /// @f$m_j@f$ The number of rows of current MPI process.
  const isvd_int_t nrow_proc;

  /// @f$n_j@f$ The number of columns of current MPI process.
  const isvd_int_t ncol_proc;

  /// @f$m_b@f$ The number of rows per MPI process.
  const isvd_int_t nrow_each;

  /// @f$n_b@f$ The number of columns per MPI process.
  const isvd_int_t ncol_each;

  /// @f$Pm_b@f$ The total number of rows in all MPI processes.
  const isvd_int_t nrow_total;

  /// @f$Pn_b@f$ The total number of columns in all MPI processes.
  const isvd_int_t ncol_total;

  /// The index range of the rows in current MPI process.
  const isvd_IdxRange rowrange;

  /// The index range of the columns in current MPI process.
  const isvd_IdxRange colrange;

  /// @f$k@f$ The desired rank of approximate SVD.
  const isvd_int_t rank;

  /// @f$p@f$ The oversampling dimension.
  const isvd_int_t over_rank;

  /// @f$l@f$ The dimension of random sketches.
  const isvd_int_t dim_sketch;

  /// @f$Nl@f$ The total dimension of random sketches.
  const isvd_int_t dim_sketch_total;

  /// @f$N@f$ The number of random sketches.
  const isvd_int_t num_sketch;

} isvd_Param;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @ingroup  core_param_module
/// Initialize the parameters.
///
inline isvd_Param isvd_createParam(
    const isvd_int_t nrow,
    const isvd_int_t ncol,
    const isvd_int_t rank,
    const isvd_int_t over_rank,
    const isvd_int_t num_sketch,
    const mpi_int_t  mpi_root,
    const MPI_Comm   mpi_comm
) {

  mpi_int_t mpi_size, mpi_rank;
  MPI_Comm_size(mpi_comm, &mpi_size);
  MPI_Comm_rank(mpi_comm, &mpi_rank);

  struct {
    isvd_int_t nrow_;
    isvd_int_t ncol_;
    isvd_int_t rank_;
    isvd_int_t over_rank_;
    isvd_int_t num_sketch_;
  } args = {nrow, ncol, rank, over_rank, num_sketch};

  MPI_Bcast(&args, sizeof(args), MPI_BYTE, mpi_root, mpi_comm);

  if ( mpi_rank == mpi_root ) {
    isvd_assert_gt(args.nrow_, 0);
    isvd_assert_gt(args.ncol_, 0);
    isvd_assert_gt(args.rank_, 0);
    isvd_assert_ge(args.over_rank_, 0);
    isvd_assert_gt(args.num_sketch_, 0);
    isvd_assert_le(args.nrow_, args.ncol_);
  }

  isvd_int_t nrow_each = (args.nrow_-1) / mpi_size + 1;
  isvd_int_t ncol_each = (args.ncol_-1) / mpi_size + 1;

  isvd_IdxRange rowrange = {mpi_rank * nrow_each, (mpi_rank+1) * nrow_each};
  isvd_IdxRange colrange = {mpi_rank * ncol_each, (mpi_rank+1) * ncol_each};
  if ( rowrange.begin > args.nrow_ ) { rowrange.begin = args.nrow_; }
  if ( rowrange.end   > args.nrow_ ) { rowrange.end   = args.nrow_; }
  if ( colrange.begin > args.ncol_ ) { colrange.begin = args.ncol_; }
  if ( colrange.end   > args.ncol_ ) { colrange.end   = args.ncol_; }

  isvd_Param param = {
    .mpi_comm         = mpi_comm,
    .mpi_size         = mpi_size,
    .mpi_rank         = mpi_rank,
    .nrow             = args.nrow_,
    .ncol             = args.ncol_,
    .nrow_proc        = rowrange.end - rowrange.begin,
    .ncol_proc        = colrange.end - colrange.begin,
    .nrow_each        = nrow_each,
    .ncol_each        = ncol_each,
    .nrow_total       = nrow_each * mpi_size,
    .ncol_total       = ncol_each * mpi_size,
    .rowrange         = rowrange,
    .colrange         = colrange,
    .rank             = args.rank_,
    .over_rank        = args.over_rank_,
    .dim_sketch       = args.rank_ + args.over_rank_,
    .dim_sketch_total = (args.rank_ + args.over_rank_) * args.num_sketch_,
    .num_sketch       = args.num_sketch_
  };

  return param;
}

#endif  // _ISVD_CORE_PARAM_H_