////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file       include/c/isvd/core/param.h
/// \brief      The parameter structure.
///
/// \author     Mu Yang <<emfomy@gmail.com>>
/// \copyright  MIT License
///

#ifndef _ISVD_CORE_PARAM_H_
#define _ISVD_CORE_PARAM_H_

#include <isvd/def.h>

#if defined(__cplusplus)
extern "C" {
#endif  // __cplusplus

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \ingroup  core_module
/// The parameters.
///
typedef struct {

  /// The MPI communicator.
  const MPI_Comm mpi_comm;

  /// \f$P\f$ The MPI size.
  const mpi_int_t mpi_size;

  /// \f$j\f$ The MPI rank.
  const mpi_int_t mpi_rank;

  /// \f$m\f$ The number of rows of the matrix.
  const isvd_int_t nrow;

  /// \f$n\f$ The number of columns of the matrix.
  const isvd_int_t ncol;

  /// \f$m_j\f$ The number of rows of current MPI process.
  const isvd_int_t nrow_proc;

  /// \f$n_j\f$ The number of columns of current MPI process.
  const isvd_int_t ncol_proc;

  /// \f$m_b = \lceil \frac{m}{P} \rceil\f$ The number of rows per MPI process.
  const isvd_int_t nrow_each;

  /// \f$n_b = \lceil \frac{n}{P} \rceil\f$ The number of columns per MPI process.
  const isvd_int_t ncol_each;

  /// \f$Pm_b\f$ The total number of rows in all MPI processes.
  const isvd_int_t nrow_total;

  /// \f$Pn_b\f$ The total number of columns in all MPI processes.
  const isvd_int_t ncol_total;

  /// The beginning index of rows in current MPI process.
  const isvd_int_t rowidxbegin;

  /// The end index of rows in current MPI process.
  const isvd_int_t rowidxend;

  /// The beginning index of columns in current MPI process.
  const isvd_int_t colidxbegin;

  /// The end index of columns in current MPI process.
  const isvd_int_t colidxend;

  /// \f$k\f$ The desired rank of approximate SVD.
  const isvd_int_t rank;

  /// \f$p\f$ The oversampling dimension.
  const isvd_int_t over_rank;

  /// \f$l\f$ The dimension of random sketches.
  const isvd_int_t dim_sketch;

  /// \f$Nl\f$ The total dimension of random sketches.
  const isvd_int_t dim_sketch_total;

  /// \f$N\f$ The number of random sketches.
  const isvd_int_t num_sketch;

} isvd_Param;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \ingroup  core_module
/// Initialize the parameters.
///
isvd_Param isvd_createParam(
    const isvd_int_t nrow, const isvd_int_t ncol, const isvd_int_t rank, const isvd_int_t over_rank,
    const isvd_int_t num_sketch, const mpi_int_t mpi_root, const MPI_Comm mpi_comm
);

typedef void (*isvd_fun_t)(isvd_Param, ...);

#if defined(__cplusplus)
}
#endif  // __cplusplus

#endif  // _ISVD_CORE_PARAM_H_
