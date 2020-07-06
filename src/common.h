#ifndef _INTERCEPTER_H_
#define _INTERCEPTER_H_ 

//#define MEASURE_TIME

#define GB_BYTES    0x40000000UL
#define MB_BYTES    0x100000UL

#define STRINGIFY(x) #x
#define CUDA_SYMBOL_STRING(x) STRINGIFY(x)
#define NVML_SYMBOL_STRING(x) STRINGIFY(x)

#define DISALLOW_COPY_AND_ASSIGN(TypeName)  \
        TypeName(const TypeName&) = delete;  \
        void operator=(const TypeName&) = delete

#ifdef MEASURE_TIME

#include <time.h>

#define TIME_START()    clock_t __time_tmp = clock()
#define TIME_END()      do {                            \
    __time_tmp = clock() - __time_tmp;                  \
    INFO_PRT("Measure time is %f ms\n", (float)__time_tmp * 1000 / CLOCKS_PER_SEC);    \
    __time_tmp = clock();                               \
    } while(0)
#else

#define TIME_START() do{} while(0)
#define TIME_END() do{} while(0)

#endif

#define INTERCEPTER_LOGO    "gpu_partition"
#define INFO_PRT(fmt,...) do{                                                           \
	printf("%s - [%s] info:" fmt, INTERCEPTER_LOGO, __func__, ##__VA_ARGS__);			\
    } while(0)
#define WARN_PRT(fmt,...) do{                                                           \
	printf("%s - [%s] warn:" fmt, INTERCEPTER_LOGO, __func__, ##__VA_ARGS__);			\
    } while(0)


#define CHECK_NVML_RETURN(func_call, ret_val) do {    \
                        nvmlReturn_t ret = (func_call); \
                        if(ret != NVML_SUCCESS) {   \
                            WARN_PRT("%s\n", nvmlErrorString(ret)); \
                            return ret_val;  \
                        }   \
                        } while(0)

#define CHECK_CUDA_RETURN(func_call, ret_val) do {    \
                        cudaError_t ret = (func_call); \
                        if(ret != cudaSuccess) {   \
                            WARN_PRT("%s\n", cudaGetErrorString(ret)); \
                            return ret_val;  \
                        }   \
                        } while(0)

#define CHECK_CU_RETURN(func_call, ret_val) do {    \
                        CUresult ret = (func_call); \
                        if(ret != CUDA_SUCCESS) {   \
                            const char *str;  \
                            real_cuGetErrorString(ret, &str);  \
                            WARN_PRT("%s\n", str);  \
                            return ret_val;  \
                        }   \
                        } while(0)

#endif //_INTERCEPTER_H_
