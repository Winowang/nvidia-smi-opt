#ifndef _DL_FUNC_H_
#define _DL_FUNC_H_

#define OVERWRITE_DLSYM

#ifdef  __cplusplus
extern "C" {
#endif

#ifdef OVERWRITE_DLSYM
void *real_dlsym(void *handle, const char *symbol);
#else
#define real_dlsym	dlsym
#endif

#ifdef  __cplusplus
}
#endif

#endif //_DL_FUNC_H_
