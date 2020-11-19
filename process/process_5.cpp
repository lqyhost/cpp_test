#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <signal.h>

static sig_atomic_t  reap;
static sig_atomic_t  terminate;

static pid_t master_pid;
static pid_t worker_pid;
static const char* szfilename = "/tmp/my_test_daemon.txt";

void signal_handler(int signo);
void worker(long i);
void init_signals();
void start_worker_processes();
bool start_daemon();

typedef struct {
    int     signo;   //需要处理的信号
    void  (*handler)(int signo);   //收到signo信号后就会回调handler方法
} signal_t;

//指定各种信号的处理函数
signal_t signals[] = {
    {SIGHUP, signal_handler},
    {SIGQUIT, signal_handler},
    {SIGTERM, signal_handler},
    {SIGALRM, signal_handler},
    {SIGINT, signal_handler},
    {SIGIO, signal_handler},
    {SIGCHLD, signal_handler},
    {SIGSYS, SIG_IGN},
    {SIGPIPE, SIG_IGN},
    {0, NULL}
};


int main(int argc, char** argv)
{
    sigset_t set;

    //初始化各信号，给个信号安装处理函数
    init_signals();

    //主进程以daemon方式运行
    start_daemon();

    //设置信号掩码，暂时屏蔽信号，后续通过sigsuspend主动获取信号
    sigemptyset(&set);
    sigaddset(&set, SIGHUP);
    sigaddset(&set, SIGQUIT);
    sigaddset(&set, SIGTERM);
    sigaddset(&set, SIGALRM);
    sigaddset(&set, SIGINT);
    sigaddset(&set, SIGIO);
    sigaddset(&set, SIGCHLD);

    if (sigprocmask(SIG_BLOCK, &set, NULL) == -1)
    {
        printf("sigprocmask failed\n");
        return 0;
    }

    sigemptyset(&set);

    //启动worker进程
    start_worker_processes();

    //主进程循环监控
    while (true)
    {
        //sigsuspend(const sigset_t *mask))用于在接收到某个信号之前, 临时用mask替换进程的信号掩码, 并暂停进程执行，直到收到信号为止。
        sigsuspend(&set);
        if (terminate)
        {
            kill(worker_pid, SIGTERM);   //通知子进程关闭
            waitpid(worker_pid, NULL, 0);   //等待回收资源，避免僵尸进程

            //主进程退出时删除测试文件
            if (0 == access(szfilename, F_OK))
            {
                remove(szfilename);
            }

            exit(0);
        }

        if (reap)
        {
            waitpid(worker_pid, NULL, 0);   //若子进程退出，需等待回收资源，避免僵尸进程
            start_worker_processes();    //拉起新的子进程
        }

        sleep(1);
    }

    return 0;
}

void signal_handler(int signo)
{
    signal_t    *sig;

    for (sig = signals; sig->signo != 0; sig++)
    {
        if (sig->signo == signo) {
            break;
        }
    }

    switch (signo)
    {
        case SIGTERM:
        case SIGINT:
        {
            terminate = 1;
            break;
        }
        case SIGCHLD:
        {
            reap = 1;
            break;
        }
        default:
        {
            break;
        }
    }
}

bool start_daemon()
{
    int fd;

    switch (fork()) {
        case -1:
            printf("fork() failed\n");
            return false;

        case 0:
            break;

        default:
            exit(0);
    }

    if (setsid() == -1) {
        printf("setsid() failed\n");
        return false;
    }

    switch (fork()) {
        case -1:
            printf("fork() failed\n");
            return false;

        case 0:
            break;

        default:
            exit(0);
    }

    umask(0);
    chdir("/");

    long maxfd;
    if ((maxfd = sysconf(_SC_OPEN_MAX)) != -1)
    {
        for (fd = 0; fd < maxfd; fd++)
        {
            close(fd);
        }
    }

    fd = open("/dev/null", O_RDWR);
    if (fd == -1) {
        printf("open(\"/dev/null\") failed\n");
        return false;
    }

    if (dup2(fd, STDIN_FILENO) == -1) {
        printf("dup2(STDIN) failed\n");
        return false;
    }

    if (dup2(fd, STDOUT_FILENO) == -1) {
        printf("dup2(STDOUT) failed\n");
        return false;
    }

    if (dup2(fd, STDERR_FILENO) == -1) {
        printf("dup2(STDERR) failed\n");
        return false;
    }

    if (fd > STDERR_FILENO) {
        if (close(fd) == -1) {
            printf("close() failed\n");
            return false;
        }
    }

    return true;
}

void worker(long i)
{
    sigset_t set;

    //子进程继承父进程对信号的屏蔽，所以这里需要解除对信号的屏蔽
    sigemptyset(&set);
    if (sigprocmask(SIG_SETMASK, &set, NULL) == -1)
    {
        printf("work sigprocmask failed\n");
        return;
    }

    FILE* pFile = fopen(szfilename, "a+");

    while (true)
    {
        if (terminate)
        {
            exit(0);
        }

        fprintf(pFile, "pid = %ld hello world\n", i);
        fflush(pFile);
        sleep(2);
    }

    if (NULL != pFile)
    {
        fclose(pFile);
        pFile = NULL;
    }
}

void init_signals()
{
    signal_t* sig;
    struct sigaction sa;

    for (sig = signals; sig->signo != 0; sig++)
    {
        memset(&sa, 0, sizeof(struct sigaction));
        sa.sa_handler = sig->handler;
        sigemptyset(&sa.sa_mask);

        if (sigaction(sig->signo, &sa, NULL) == -1)
        {
            printf("sigaction error\n");
            return;
        }
    }
}


void start_worker_processes()
{
    pid_t pid;

    pid = fork();
    switch (pid)
    {
        case -1:
            printf("exec fork failure\n");
            return;
        case 0:
            worker((long)getpid());
            break;
        default:
            worker_pid = pid;
            break;
    }
}

