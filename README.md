# nvidia-opt
The improved nvidia-smi implemtation whcih can display the process pid in container or pod.

The usage:
1. copy the libnvidia_opti.so to the contianer/pod's  path:  /usr/local/lib/ 
2. export LD_PRELOAD=/usr/local/lib/libnvidia_opti.so 

After lanuching your gpu process, you can use nvidia-smi to check the process pid like below

------------------------------------------------------------------------------------------------

root@8adc4c5d7a5c:/home# nvidia-smi                                             
Mon Jul  6 03:04:15 2020  
                                                      
+-----------------------------------------------------------------------------+ 

| NVIDIA-SMI 430.40       Driver Version: 430.40       CUDA Version: 10.1     | 

|-------------------------------+----------------------+----------------------+ 

| GPU  Name        Persistence-M| Bus-Id        Disp.A | Volatile Uncorr. ECC | 

| Fan  Temp  Perf  Pwr:Usage/Cap|         Memory-Usage | GPU-Util  Compute M. | 

|===============================+======================+======================| 

|   0  Tesla P40           Off  | 00000000:3B:00.0 Off |                    0 | 

| N/A   39C    P0    56W / 250W |   4025MiB / 15289MiB |      2%      Def
ault | 

+-------------------------------+----------------------+----------------------+ 

|   1  Tesla P40           Off  | 00000000:AF:00.0 Off |                    0 | 

| N/A   26C    P8    10W / 250W |      0MiB / 15289MiB |      0%      Default | 

+-------------------------------+----------------------+----------------------+ 


                                                                                
+-----------------------------------------------------------------------------+ 

| Processes:                                                       GPU Memory | 

|  GPU       PID   Type   Process name                             Usage      | 

|=============================================================================|

|    0     26455      C   ./mem_cuda_ctl                              4025MiB | 

+-----------------------------------------------------------------------------+ 


-------------------------------------------------------------------------------------------------

