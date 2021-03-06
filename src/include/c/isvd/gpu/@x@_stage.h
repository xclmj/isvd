////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file       include/c/isvd/gpu/@x@_stage.h
/// \brief      The iSVD stages with GPU support.
///
/// \author     Mu Yang <<emfomy@gmail.com>>
/// \copyright  Copyright (c) 2018 Mu Yang. All rights reserved.
/// \license    This project is released under the \ref Readme_License "MIT License".
///

#ifndef ISVD_GPU_@X@_STAGE_H_
#define ISVD_GPU_@X@_STAGE_H_

#include <isvd/def.h>
#include <isvd/core/param.h>

#if defined(__cplusplus)
extern "C" {
#endif  // __cplusplus

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \ingroup  c_gpu_stage_module
/// \brief  GPU Gaussian Projection Sketching (@xname@ precision).
///
/// \copydetails  isvd_@x@SketchGaussianProjection
///
/// \attention  Set \ref isvd_gpu_memory_limit as the limit of GPU memory usage.
///
void isvd_@x@SketchGaussianProjection_gpu(
    const isvd_Param param, const @xtype_____@ *argv, const isvd_int_t argc, @xtype_____@ *retv, const isvd_int_t retc,
    const char dista, const char ordera, const @xtype_____@ *a, const isvd_int_t lda,
    @xtype_____@ *yst, const isvd_int_t ldyst, const isvd_int_t seed, const mpi_int_t mpi_root
);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \ingroup  c_gpu_stage_module
/// \brief  GPU Tall-Skinny QR Postprocessing (@xname@ precision).
///
/// \copydetails  isvd_@x@PostprocessTallSkinnyQr
///
/// \attention  Set \ref isvd_gpu_memory_limit as the limit of GPU memory usage.
///
void isvd_@x@PostprocessTallSkinnyQr_gpu(
    const isvd_Param param, const @xtype_____@ *argv, const isvd_int_t argc, @xtype_____@ *retv, const isvd_int_t retc,
    const char dista, const char ordera,
    const @xtype_____@ *a, const isvd_int_t lda, const @xtype_____@ *qt, const isvd_int_t ldqt,
          @xtype_____@ *s, @xtype_____@ *ut, const isvd_int_t ldut, @xtype_____@ *vt, const isvd_int_t ldvt,
    const mpi_int_t ut_root, const mpi_int_t vt_root
);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \ingroup  c_gpu_stage_module
/// \brief  GPU Gramian Postprocessing (@xname@ precision).
///
/// \copydetails  isvd_@x@PostprocessGramian
///
/// \attention  Set \ref isvd_gpu_memory_limit as the limit of GPU memory usage.
///
void isvd_@x@PostprocessGramian_gpu(
    const isvd_Param param, const @xtype_____@ *argv, const isvd_int_t argc, @xtype_____@ *retv, const isvd_int_t retc,
    const char dista, const char ordera,
    const @xtype_____@ *a, const isvd_int_t lda, const @xtype_____@ *qt, const isvd_int_t ldqt,
          @xtype_____@ *s, @xtype_____@ *ut, const isvd_int_t ldut, @xtype_____@ *vt, const isvd_int_t ldvt,
    const mpi_int_t ut_root, const mpi_int_t vt_root
);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \ingroup  c_gpu_stage_module
/// \brief  GPU Symmetric Postprocessing (@xname@ precision).
///
/// \copydetails  isvd_@x@PostprocessSymmetric
///
/// \attention  Set \ref isvd_gpu_memory_limit as the limit of GPU memory usage.
///
void isvd_@x@PostprocessSymmetric_gpu(
    const isvd_Param param, const @xtype_____@ *argv, const isvd_int_t argc, @xtype_____@ *retv, const isvd_int_t retc,
    const char dista, const char ordera,
    const @xtype_____@ *a, const isvd_int_t lda, const @xtype_____@ *qt, const isvd_int_t ldqt,
          @xtype_____@ *s, @xtype_____@ *ut, const isvd_int_t ldut, @xtype_____@ *vt, const isvd_int_t ldvt,
    const mpi_int_t ut_root, const mpi_int_t vt_root
);

#if defined(__cplusplus)
}
#endif  // __cplusplus

#endif  // ISVD_GPU_@X@_STAGE_H_
