#include <cstdlib>
#include <string>
#include <iostream>
#include <unordered_map>
#include <regex>
#include <vector> 

#include "procps.h"
#include "readproc.h"
#include "proc_info.h"


int get_host_pid(int pid)
{
	static std::unordered_map<int, int> pid_map;
	auto it = pid_map.find(pid);
	if(it != pid_map.end())
	{
		//std::cout << "Find host pid:" << it->second << std::endl;
		return it->second;
	}
	else
	{
		int host_pid;
		char filename[32], line[128];

		snprintf(filename, 32, "/proc/%d/sched", pid);
		FILE *fp;
		fp = fopen(filename, "r");
		if(fp == NULL)
		{
			std::cout << "Error:get host pid fail!!!!!!!!!!!!!!!!!!\n";
			return 0;
		}
		fgets(line, sizeof(line), fp);

        std::string pid_name = line;
        //std::cout << "  line string  "  << pid_name << std::endl;
        pid_name=pid_name.substr(0,10);

        // exclude the nvidia-smi process
        if( pid_name!="nvidia-smi"){ 

		std::regex sp ("\\((\\d+)\\, #threads:");
		std::smatch sr;
		std::string s(line);

		if(std::regex_search(s, sr, sp))
		{
#if 0
			for(auto m:sr) 
			std::cout << m << std::endl;
#endif
		}
		else
		{
			std::cout << "Error:get host pid fail!!!!!!!!!!!!!!!!!!\n";
			return 0;
		}

		host_pid = std::stoi(sr[1]);
		pid_map.insert(std::pair<int, int>(pid, host_pid));	
	//	std::cout << "Get host pid:" << host_pid << std::endl;
        }
        else 
        {
        return 0;
        }

		return host_pid;
	}
}



int get_system_all_host_pids(std::unordered_set<int>& pids)
{
    //proc_t **procs_pptr = readproctab(PROC_FILLSTAT);
    //proc_t **procs_pptr = readproctab(PROC_FILLSTATUS);
    proc_t **procs_pptr = readproctab(0);
    
    for(; *procs_pptr; procs_pptr++) 
    {
     const proc_t *const tmp = *procs_pptr;
     pids.insert(get_host_pid(tmp->tid));
     //std::cout << "tgid:" << tmp->tgid << " tid:" << tmp->tid << std::endl;
     //std::cout << "host pid:" << get_host_pid(tmp->tid) << std::endl;
    }

    return 0;
}



int get_system_all_pids(std::vector<int>& pid_container, std::vector<int>& pid_host)
{
    //proc_t **procs_pptr = readproctab(PROC_FILLSTAT);
    //proc_t **procs_pptr = readproctab(PROC_FILLSTATUS);
    proc_t **procs_pptr = readproctab(0);
    
//    std::vector<int> pid_container;
//    std::vector<int> pid_host;

    for(; *procs_pptr; procs_pptr++) 
    {
        const proc_t *const tmp = *procs_pptr;
        //std::cout << "tgid:" << tmp->tgid << " tid:" << tmp->tid << std::endl;
        //std::cout << "host pid:" << get_host_pid(tmp->tid) << std::endl;
  
        pid_container.push_back(tmp->tid);
        pid_host.push_back(get_host_pid(tmp->tid));
    }

    //for(int i=0; i<pid_container.size(); i++)
    //        {
    //           std::cout<<"Container_pid(func): " << pid_container[i] << ", Host_pid(func):  "<< pid_host[i]<< std::endl;
    //        }

    return 0;
}



