#ifndef _PROC_INFO_H_
#define _PROC_INFO_H_

#include <unordered_set>
#include <vector>

#ifdef  __cplusplus
extern "C" {
#endif

int get_host_pid(int pid);
int get_system_all_host_pids(std::unordered_set<int>& pids);
int get_system_all_pids(std::vector<int>& pid_container, std::vector<int>& pid_host);

#ifdef  __cplusplus
}
#endif

#endif //_PROC_INFO_H_

