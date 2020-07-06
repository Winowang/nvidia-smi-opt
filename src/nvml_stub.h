#ifndef _NVML_STUB_H_
#define _NVML_STUB_H_

#include <nvml.h>
//#include <cuda_runtime.h>


void init_nvml_lib(void);

void des_nvml_lib(void);

nvmlReturn_t DECLDIR real_nvmlDeviceGetComputeRunningProcesses(nvmlDevice_t device, unsigned int *infoCount, nvmlProcessInfo_t *infos);

#endif
