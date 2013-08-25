12 08 20 网上获取系统的进程信息大多数用的是CreateToolhelp32Snapshot函数  
关于CreateToolhelp32Snapshot：  
CreateToolhelp32Snapshot函数是给系统状态来一个'快照'，听起很奇怪，为什么获取系统的信息要用'快照'这种方式？我也不懂。  
HANDLE WINAPI CreateToolhelp32Snapshot(DWORD dwFlags, DWORD th32ProcessID);  
大多信息都在TlHelp32.h头文件里，对于不同的dwFlags，可以分别获取不用的信息：  
TH32CS_SNAPPROCESS   进程信息
TH32CS_SNAPHEAPLIST   堆信息
TH32CS_SNAPTHREAD    th32ProcessID的线程信息
TH32CS_SNAPMODULE    th32ProcessID的模块信息，在64位系统里：64位的app里使用拿到64位的模块句柄，32位的app拿到32位的模块句柄  
TH32CS_SNAPMODULE32  th32ProcessID的模块信息，在64位系统里，拿到32位的模块句柄  
TH32CS_SNAPALL (TH32CS_SNAPHEAPLIST | TH32CS_SNAPPROCESS | TH32CS_SNAPTHREAD | TH32CS_SNAPMODULE)  
TH32CS_INHERIT  显示可以被继承的（Snapshot）handle??  
拿到'快照'后，就用以下的api来分析'快照'
Heap32First  
Heap32ListFirst  
Heap32ListNext  
Heap32Next   
Module32First  
Module32Next   
Process32First  
Process32Next  
Thread32First  
Thread32Next  

12 08 21 写出来的程序既依赖VC的库，又依赖Qt的库，真不是一样好事情。    
12 08 24