#include <stdio.h>
#include <unistd.h>

int gval = 10;

int main()
{
    pid_t pid;
    int lval = 20;
    gval++, lval += 5;

    pid = fork();
    if (pid == 0) // if child process
    {
        printf("child process\n");
        gval += 2, lval += 2;
    }
    else
    {
        printf("parent process,pid: %d\n", pid);
        gval -= 2, lval -= 2;
    }
    pid = fork();
    if (pid == 0) // if child process
    {
        printf("child process\n");
        gval += 2, lval += 2;
    }
    else
    {
        printf("parent process,pid: %d\n", pid);
        gval -= 2, lval -= 2;
    }
    if (pid == 0)
        printf("child proc:[%d, %d]\n", gval, lval);
    else
        printf("parent proc [%d, %d]\n", gval, lval);

    sleep(10);

    return 0;
}