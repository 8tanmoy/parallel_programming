/*
 * Copyright 1993-2006 NVIDIA Corporation.  All rights reserved.
 *
 * NOTICE TO USER:   
 *
 * This source code is subject to NVIDIA ownership rights under U.S. and 
 * international Copyright laws.  
 *
 * This software and the information contained herein is PROPRIETARY and 
 * CONFIDENTIAL to NVIDIA and is being provided under the terms and 
 * conditions of a Non-Disclosure Agreement.  Any reproduction or 
 * disclosure to any third party without the express written consent of 
 * NVIDIA is prohibited.     
 *
 * NVIDIA MAKES NO REPRESENTATION ABOUT THE SUITABILITY OF THIS SOURCE 
 * CODE FOR ANY PURPOSE.  IT IS PROVIDED "AS IS" WITHOUT EXPRESS OR 
 * IMPLIED WARRANTY OF ANY KIND.  NVIDIA DISCLAIMS ALL WARRANTIES WITH 
 * REGARD TO THIS SOURCE CODE, INCLUDING ALL IMPLIED WARRANTIES OF 
 * MERCHANTABILITY, NONINFRINGEMENT, AND FITNESS FOR A PARTICULAR PURPOSE.   
 * IN NO EVENT SHALL NVIDIA BE LIABLE FOR ANY SPECIAL, INDIRECT, INCIDENTAL, 
 * OR CONSEQUENTIAL DAMAGES, OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS 
 * OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE 
 * OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE 
 * OR PERFORMANCE OF THIS SOURCE CODE.  
 *
 * U.S. Government End Users.  This source code is a "commercial item" as 
 * that term is defined at 48 C.F.R. 2.101 (OCT 1995), consisting  of 
 * "commercial computer software" and "commercial computer software 
 * documentation" as such terms are used in 48 C.F.R. 12.212 (SEPT 1995) 
 * and is provided to the U.S. Government only as a commercial end item.  
 * Consistent with 48 C.F.R.12.212 and 48 C.F.R. 227.7202-1 through 
 * 227.7202-4 (JUNE 1995), all U.S. Government End Users acquire the 
 * source code with only those rights set forth herein.
 */

#ifndef _VECTOR_REDUCTION_KERNEL_H_
#define _VECTOR_REDUCTION_KERNEL_H_

#define NUM_ELEMENTS 1024


// **===----------------- Modify this function ---------------------===**
//! @param g_idata  input data in global memory
//                  result is expected in index 0 of g_idata
//! @param n        input number of elements to scan from input data
// **===------------------------------------------------------------------===**
template <unsigned int blockSize>
 __global__ void reduce(float *g_idata, float *g_odata, unsigned int n)
 {
 	extern __shared__ float sdata[];

 	unsigned int tid = threadIdx.x;
 	unsigned int i = blockIdx.x*(blockSize*2) + tid;
 	unsigned int gridSize = blockSize*2*gridDim.x;
 	sdata[tid] = 0.0;

 	while(i < n) {sdata[tid] += g_idata[i] + g_idata[i + blockSize]; i += gridSize; }
 	__syncthreads();

 	if (blockSize >= 512) {if (tid < 256) { sdata[tid] += sdata[tid + 256];} __syncthreads(); }
 	if (blockSize >= 256) {if (tid < 128) { sdata[tid] += sdata[tid + 128];} __syncthreads(); }
 	if (blockSize >= 128) {if (tid <  64) { sdata[tid] += sdata[tid +  64];} __syncthreads(); }
 	if (tid < 32)
 	{
 		if (blockSize >= 64) sdata[tid] += sdata[tid + 32];
 		if (blockSize >= 32) sdata[tid] += sdata[tid + 16];
 		if (blockSize >= 16) sdata[tid] += sdata[tid +  8];
 		if (blockSize >=  8) sdata[tid] += sdata[tid +  4];
 		if (blockSize >=  4) sdata[tid] += sdata[tid +  2];
 		if (blockSize >=  2) sdata[tid] += sdata[tid +  1];
 	}
 	if (tid == 0) g_odata[blockIdx.x] = sdata[0];
 }

#endif // #ifndef _VECTOR_REDUCTION_KERNEL_H_