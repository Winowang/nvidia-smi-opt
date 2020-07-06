#include <dlfcn.h>
#include "dl_func.h"
#include <string>

#ifdef OVERWRITE_DLSYM

extern "C" void *__libc_dlsym(void *__map, const char *__name);
//extern "C" void *_dl_sym(void *handle, const char *name, void *who);
void *real_dlsym(void *handle, const char *symbol)
{
    using FuncPtr = void *(*)(void *, const char *);
    
    void* dl_hdl_orign=dlopen("libdl.so", RTLD_LAZY);
    //dl_hdl_version=dlopen("libdl.so.2", RTLD_LAZY);    
    
    char* dl_name;
    if(dl_hdl_orign) dl_name=(char*)"libdl.so";
    else dl_name =(char*)"libdl.so.2";

    //static void *dl_hdl = dlopen("libdl.so", RTLD_LAZY);
    static void *dl_hdl = dlopen(dl_name, RTLD_LAZY);
    static FuncPtr func_ptr = reinterpret_cast<FuncPtr>(__libc_dlsym(dl_hdl, "dlsym"));
	return func_ptr(handle, symbol);
}
#endif

