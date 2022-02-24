/**
 * @file pipe2.c
 * @author Listening (120311070@qq.com)
 * @brief 存在问题的管道
 * @version 0.1
 * @date 2022-02-20
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include <stdio.h>
#include <unistd.h>
#define BUF_SIZE 30
int main(int argc,char* argv[])
{
    int fds[2];
    char str1[] = "Who are you?";
    char str2[] = "Thank you for your message";
    char buf[BUF_SIZE];
    pid_t pid;

    pipe(fds);
    pid  = fork();
    if(pid  == 0)
    {
        write(fds[1],str1,sizeof(str1));
        sleep(2);//若注释，下列的read将读回自己write的数据
        read(fds[0],buf,BUF_SIZE);
        printf("Child proc output:%s \n",buf);
    }
    else
    {
        read(fds[0],buf,BUF_SIZE);
        printf("Parent proc output:%s\n",buf);
        write(fds[1],str2,sizeof(str2));
        sleep(3);
    }
    
    return 0;
}
