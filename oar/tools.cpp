#include "./tools.h"
/**
 * @brief Construct a new sem::sem object
 *
 * semget
 * semctl
 * semop
 *
 */
namespace oar
{

  void SleepMillis(long time) {
#ifdef WIN32
    _sleep(time);
#elif linux
    usleep(1000 * time);
#endif
  }

    sem::sem()
    {
        m_semid = -1;
        m_sem_flg = SEM_UNDO;
    }
    bool sem::init(key_t key, unsigned short value, short sem_flg)
    {
        if (m_semid != -1)
            return false;

        m_sem_flg = sem_flg;

        if ((m_semid = semget(key, 1, 0666)) == -1)
        {
            // ENOENT 2 没有那个文件或目录
            printf("errno is %d\n", errno);
            if (errno == 2)
            {
                if ((m_semid = semget(key, 1, 0666 | IPC_CREAT | IPC_EXCL)) == -1)
                {
                    if (errno != EEXIST)
                    {
                        perror("1");
                        return false;
                    }
                    if ((m_semid = semget(key, 1, 0666)) == -1)
                    {
                        perror("2");
                        return false;
                    }

                    return true;
                }
                //设置信号量初始值
                union semun sem_union;
                sem_union.val = value;
                if (semctl(m_semid, 0, SETVAL, sem_union) < 0)
                {
                    perror("3");
                    return false;
                }
            }
            else
                return false;
        }
        return true;
    }

    bool sem::P(sem_op_t sem_op)
    {
        if (m_semid == -1)
            return false;
        struct sembuf sem_b
        {
            0,          //信号量编号，0代表第一个信号量
                sem_op, // < 0
                m_sem_flg
        };
        if (semop(m_semid, &sem_b, 1) == -1)
        {
            perror("sem::P() --> semop()");
            return false;
        }

        return true;
    }

    bool sem::V(sem_op_t sem_op)
    {
        if (m_semid == -1)
            return false;
        struct sembuf sem_b
        {
            0,          //信号量编号，0代表第一个信号量
                sem_op, // > 0
                m_sem_flg
        };
        if (semop(m_semid, &sem_b, 1) == -1)
        {
            perror("sem::V() --> semop()");
            return false;
        }

        return true;
    }

    // 获取信号量的值，成功返回信号量的值，失败返回-1。
    int sem::value()
    {
        return semctl(m_semid, 0, GETVAL);
    }

    bool sem::destroy()
    {
        if (m_semid == -1)
            return false;

        if (semctl(m_semid, 0, IPC_RMID) == -1)
        {
            printf("semctl false\n");
            perror("destroy semctl()");
            return false;
        }

        return true;
    }

    sem::~sem()
    {
    }

    CPActive::CPActive()
    {
        m_shmid = 0;
        m_pos = -1;
        m_shm = 0;
    }
    // 把当前进程的心跳信息加入共享内存进程组中。
    bool CPActive::AddPInfo(const int timeout, const char *pname)
    {
        if (m_pos != -1)
            return true;

        if (m_sem.init(SEMKEYP) == false) // 初始化信号量。
        {
            printf("创建/获取信号量(%x)失败。\n", SEMKEYP);
            return false;
        }

        // 创建/获取共享内存，键值为SHMKEYP，大小为MAXNUM_PROCESS个st_procinfo结构体的大小。
        if ((m_shmid = shmget((key_t)SHMKEYP, MAXNUM_PROCESS * sizeof(struct st_procinfo), 0666 | IPC_CREAT)) == -1)
        {

            printf("创建/获取共享内存(%x)失败。\n", SHMKEYP);

            return false;
        }

        // 将共享内存连接到当前进程的地址空间。
        m_shm = (struct st_procinfo *)shmat(m_shmid, 0, 0);

        struct st_procinfo stprocinfo; // 当前进程心跳信息的结构体。
        memset(&stprocinfo, 0, sizeof(stprocinfo));

        stprocinfo.pid = getpid();    // 当前进程号。
        stprocinfo.timeout = timeout; // 超时时间。
        stprocinfo.atime = time(0);   // 当前时间。
        /*STRNCPY(stprocinfo.pname, sizeof(stprocinfo.pname), pname, 50); // 进程名。*/

        strncpy(stprocinfo.pname, pname, sizeof(stprocinfo.pname));
        // 进程id是循环使用的，如果曾经有一个进程异常退出，没有清理自己的心跳信息，
        // 它的进程信息将残留在共享内存中，不巧的是，当前进程重用了上述进程的id，
        // 这样就会在共享内存中存在两个进程id相同的记录，守护进程检查到残留进程的
        // 心跳时，会向进程id发送退出信号，这个信号将误杀当前进程。

        // 如果共享内存中存在当前进程编号，一定是其它进程残留的数据，当前进程就重用该位置。
        for (int i = 0; i < MAXNUM_PROCESS; i++)
        {
            if ((m_shm + i)->pid == stprocinfo.pid)
            {
                m_pos = i;
                break;
            }
        }

        m_sem.P(); // 给共享内存上锁。

        if (m_pos == -1)
        {
            // 如果m_pos==-1，共享内存的进程组中不存在当前进程编号，找一个空位置。
            for (int i = 0; i < MAXNUM_PROCESS; i++)
                if ((m_shm + i)->pid == 0)
                {
                    m_pos = i;
                    break;
                }
        }

        if (m_pos == -1)
        {
            printf("共享内存空间已用完。\n");
            m_sem.V(); // 解锁
            return false;
        }

        // 把当前进程的心跳信息存入共享内存的进程组中。
        memcpy(m_shm + m_pos, &stprocinfo, sizeof(struct st_procinfo));

        m_sem.V(); // 解锁。

        return true;
    }

    // 更新共享内存进程组中当前进程的心跳时间。
    bool CPActive::UptATime()
    {
        if (m_pos == -1)
            return false;

        (m_shm + m_pos)->atime = time(0);

        return true;
    }

    CPActive::~CPActive()
    {
        // 把当前进程从共享内存的进程组中移去。
        if (m_pos != -1)
            memset(m_shm + m_pos, 0, sizeof(struct st_procinfo));

        // 把共享内存从当前进程中分离。
        if (m_shm != 0)
            shmdt(m_shm);
    }
} // namespace oar
