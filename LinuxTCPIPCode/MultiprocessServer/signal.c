/**
 * @file signal.c
 * @author Listening
 * @brief 产生信号时，为了调用信号处理器，将唤醒由于sleep（）进入阻塞的进程。而且，一旦被唤醒，将不会再进入睡眠状态。
 * @version 0.1
 * @date 2022-02-18
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include <stdio.h>
#include <unistd.h>
#include <signal.h>

void timeout(int sig)
{
    if (sig == SIGALRM)
    {
        puts("Time out");
    }
    alarm(5);
}
void keycontrol(int sig)
{
    if (sig == SIGINT)
    {
        puts("CTRL + C pressed");
    }
}

int main()
{
    int i = 0;
    signal(SIGALRM, timeout);
    signal(SIGINT, keycontrol);
    alarm(5);
    for (int i = 0; i < 3; i++)
    {
        puts("wait...");
        sleep(100);
    }
    return 0;
}