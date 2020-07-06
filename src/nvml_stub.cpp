#include <cassert>

#include <dlfcn.h>
#include <stdio.h>

#include "common.h"
#include "system/dl_func.h"
#include "nvml_stub.h"

void* dl_hdl_nvml;

void init_nvml_lib(void)
{
    //dl_hdl_nvml= dlopen("libnvidia-ml.so", RTLD_LAZY);
    //dl_hdl_nvml= dlopen("/usr/lib/x86_64-linux-gnu/libnvidia-ml.so.1", RTLD_LAZY);
    //if(!dl_hdl_nvml) {printf("libcuda.so do not  exist! \n");}
   
    dl_hdl_nvml= dlopen("/usr/lib/x86_64-linux-gnu/libnvidia-ml.so.1", RTLD_LAZY);
    if(!dl_hdl_nvml){dl_hdl_nvml= dlopen("/usr/lib64/libnvidia-ml.so.1", RTLD_LAZY);
    if(!dl_hdl_nvml){dl_hdl_nvml= dlopen("/usr/lib/nvidia/libnvidia-ml.so.1", RTLD_LAZY);
    if(!dl_hdl_nvml){dl_hdl_nvml= dlopen("/usr/lib64/nvidia/libnvidia-ml.so.1", RTLD_LAZY);
    if(!dl_hdl_nvml){dl_hdl_nvml= dlopen("/usr/lib/libnvidia-ml.so.1", RTLD_LAZY);}}}}

    //if(!dl_hdl_nvml) {printf("libnvidia-ml.so does not exist! \n");}
}

void des_nvml_lib(void)
{
    if(dl_hdl_nvml){
    dlclose(dl_hdl_nvml);}
    //else
    //printf("libnvidia-ml.so does not exist! \n");
}

nvmlReturn_t DECLDIR real_nvmlDeviceGetComputeRunningProcesses(nvmlDevice_t device, unsigned int *infoCount, nvmlProcessInfo_t *infos)
{
  using FuncPtr = nvmlReturn_t (DECLDIR *)(nvmlDevice_t, unsigned int*, nvmlProcessInfo_t*);
  static auto func_ptr = reinterpret_cast<FuncPtr>(real_dlsym(dl_hdl_nvml, NVML_SYMBOL_STRING(nvmlDeviceGetComputeRunningProcesses)));             
  assert(func_ptr);                                                                                                          
  return func_ptr(device,infoCount,infos);                                                                                               
}
