# liunx_program_learn
goudan的linux学习库

### - file : linux 文件系统
    - file_open :   打开文件
    - file_write:   写入文件
    - file_read:    读取文件
    - file_stat:    文件状态
    - file_copy     文件拷贝
    - lseek         文件指针移动
    - mmapRead      文件映射虚拟内存读取文件
    - mmapWrite     文件映射虚拟内存写入文件
### - process: linux 进程
    - test.c        用于观察进程的测试文件 理解进程概念
    - fork_1.c      创建进程: 三种方式： system fork exec族 这里是fork创建子进程
    - fork_work.c   创建父进程和子进程， 父进程负责读取输入并控制子进程，子进程负责输出当前事件
