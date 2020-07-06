#include <cstdlib>
#include <cstdio>
#include <cassert>
#include <iostream>

#include <nvml.h>

#include <mutex>
#include <unordered_map>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <string>         
#include <regex>          
                          
#include <fstream>        

#include <unistd.h>       
#include <dlfcn.h>        
                          
#include <nvml.h>         
#include "common.h"
#include "nvml_stub.h"
#include "system/proc_info.h"
#include "system/dl_func.h" 


#ifdef OVERWRITE_DLSYM                                                                                        
static std::unordered_map<std::string, void *> dlsym_func_map_nv = {                                          
    {NVML_SYMBOL_STRING(nvmlDeviceGetComputeRunningProcesses), (void *)nvmlDeviceGetComputeRunningProcesses}  
};                                                                                                            
#endif                                                                                                        
                                                                                                              
static __attribute__((constructor)) void                                                                      
intercepter_constructor( void )                                                                               
{                                                                                                             
                                                                                                             
init_nvml_lib(); 

}


/* intercepter destructor */                    
static __attribute__((destructor)) void         
intercepter_destructor(void)                    
{                                               
    des_nvml_lib();                             
    //INFO_PRT("cuda intercept destroy\n");     
}                                               
                                                

#pragma GCC visibility push(default)

#ifdef OVERWRITE_DLSYM                                                       
void *dlsym(void *handle, const char *symbol)                                
{                                                                            
    //INFO_PRT("dlsym %s\n", symbol);                                        
                                                                             
    auto iter = dlsym_func_map_nv.find(symbol);                              
                                                                             
    if(iter != dlsym_func_map_nv.end())  return iter->second;                
    else  return real_dlsym(handle, symbol);                                 
                                                                            
}                                                                            
#endif                                                                       


int Gpu_Running_Pid( std::vector<int>& pid_container, std::vector<long long int>& pid_container_mem, std::vector<int>& pid_host, int gpu_idx)
{
    // First initialize NVML library
    nvmlReturn_t result;
    result = nvmlInit();
    if (NVML_SUCCESS != result)
    {
      printf("Failed to initialize NVML: %s\n", nvmlErrorString(result));
    }

    
    int TASK_NUM=16;
    unsigned int num = TASK_NUM;
    nvmlProcessInfo_t pi[TASK_NUM] = {};
    std::unordered_set<int> pids;


    // NVML API to get index and running process
    nvmlDevice_t hd_gpu;
    result=nvmlDeviceGetHandleByIndex(gpu_idx, &hd_gpu);
    if (NVML_SUCCESS != result)
      {
       printf("Failed to get handle for device: %s\n", nvmlErrorString(result));
       return 0;
      }

    result = real_nvmlDeviceGetComputeRunningProcesses(hd_gpu, &num, pi);
    if (NVML_SUCCESS != result)
      {
       printf("Failed to get name of device %s\n", nvmlErrorString(result));
       return 0;
      }
   
    // get all pids of host and contianers
    std::vector<int> pid_c;
    std::vector<int> pid_h;
    get_system_all_pids(pid_c, pid_h);
    
    //for(int i=0; i<pid_c.size(); i++)                                                                                    
    //  {                                                                                                                    
    //   std::cout<<"Container_pid(nvml): " << pid_c[i] << ", Host_pid(nvml):  "<< pid_h[i]<< std::endl;
    //  }

    // push running pid and name to container vector
    std::vector<std::string> process_name;
    for(int i = 0; i < num; i++)
    {
     for(int j=0; j<pid_h.size(); j++)                                                                                    
      {                                                                                                                         
       //std::cout<<"Host_pid(all): " << pi[i].pid << std::endl;      
       if(pid_h[j]==pi[i].pid)
        {  
         // get the process pid
         //std::cout<<"Container_pid(running): " << pid_c[j] << ", Host_pid(running):  "<< pid_h[j]<< std::endl;   
         pid_container.push_back(pid_c[j]);
         pid_host.push_back(pid_h[j]);

         // get the process memory
         pid_container_mem.push_back(pi[i].usedGpuMemory);
         
         // get the process name
        // unsigned int  LENGTH=32;
        // char name_container[LENGTH];
        // result =  nvmlSystemGetProcessName(pid_c[j], name_container, LENGTH);
        // if (NVML_SUCCESS != result)                                                     
        //   {                                                                             
        //    printf("Failed to get name of device %s\n", nvmlErrorString(result));        
        //    return 0;                                                                    
        //   }        
        // std::string name_tmp = name_container;
        // process_name.push_back(name_tmp);
         //std::cout<<"Pid_porcess_name " << name_container << std::endl;      
        }
      }
    }

  return 0;
}
    

nvmlReturn_t DECLDIR  nvmlDeviceGetComputeRunningProcesses ( nvmlDevice_t device, unsigned int* infoCount, nvmlProcessInfo_t* infos )    
{
   int idx, idx_old;                                                                   
   if(nvmlDeviceGetIndex(device, (unsigned int *)&idx) != NVML_SUCCESS)                
   {                                                                                   
       return NVML_ERROR_UNKNOWN;                                                      
   }                                                                                   
 
   std::vector<int> pid_container;                              
   std::vector<long long int> pid_container_mem;
   std::vector<int> pid_host;                                   
   Gpu_Running_Pid(pid_container, pid_container_mem, pid_host, idx);                 
  
   *infoCount = pid_container.size();
   for (int i=0; i<pid_container.size(); i++)
     {
       infos[i].pid = pid_container[i];
       infos[i].usedGpuMemory = pid_container_mem[i];
       //std::cout << "PID        "   << pid_container[i] << std::endl;
     }

   return NVML_SUCCESS;
}

#pragma GCC visibility pop

