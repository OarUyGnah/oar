#ifndef __TOOLS_H
#define __TOOLS_H
#include <semaphore.h>
#include <sys/types.h>
#include <sys/sem.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <errno.h>
#include <stdio.h>

namespace oar
{

  void SleepMillis(long time);



#define MAXNUM_PROCESS 1000 // 最大的进程数量。
#define SHMKEYP 0x5095      // 共享内存的key。
#define SEMKEYP 0x5095      // 信号量的key。


  // 信号量。
    class sem
    {
        using sem_op_t = short int;

    public:
        sem();
        // 如果信号量已存在，获取信号量；如果信号量不存在，则创建它并初始化为value。
        bool init(key_t key, unsigned short value = 1, short sem_flg = SEM_UNDO);
        bool P(sem_op_t sem_op = -1); // 信号量的P操作。
        bool V(sem_op_t sem_op = 1);  // 信号量的V操作。
        int value();                  // 获取信号量的值，成功返回信号量的值，失败返回-1。
        bool destroy();               // 销毁信号量。
        ~sem();

    private:
        union semun // 用于信号量操作的共同体。
        {
            int val;
            struct semid_ds *buf;
            unsigned short *arry;
        };

        int m_semid; // 信号量描述符。

        // 如果把sem_flg设置为SEM_UNDO，操作系统将跟踪进程对信号量的修改情况，
        // 在全部修改过信号量的进程（正常或异常）终止后，操作系统将把信号量恢
        // 复为初始值（就像撤消了全部进程对信号的操作）。
        // 如果信号量用于表示可用资源的数量（不变的），设置为SEM_UNDO更合适。
        // 如果信号量用于生产消费者模型，设置为0更合适。
        // 注意，网上查到的关于sem_flg的用法基本上是错的，一定要自己动手多测试。
        short m_sem_flg;
    };
    // 查看共享内存：  ipcs -m
    // 删除共享内存：  ipcrm -m shmid
    // 查看信号量：    ipcs -s
    // 删除信号量：    ipcrm sem semid

    // 进程心跳操作类。
    class CPActive
    {

    public:
        CPActive(); // 初始化成员变量。

        // 把当前进程的心跳信息加入共享内存进程组中。
        bool AddPInfo(const int timeout, const char *pname = 0);

        // 更新共享内存进程组中当前进程的心跳时间。
        bool UptATime();

        ~CPActive(); // 从共享内存中删除当前进程的心跳记录。
        struct st_procinfo
        {
            int pid;        // 进程id。
            char pname[51]; // 进程名称，可以为空。
            int timeout;    // 超时时间，单位：秒。
            time_t atime;   // 最后一次心跳的时间，用整数表示。
        };

    private:
        sem m_sem;          // 用于给共享内存加锁的信号量id。
        int m_shmid;        // 共享内存的id。
        int m_pos;          // 当前进程在共享内存进程组中的位置。
        st_procinfo *m_shm; // 指向共享内存的地址空间。
    };
} // namespace oar

#endif
