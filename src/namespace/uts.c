#define _GNU_SOURCE

#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <sched.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>

#define STACK_SIZE (1024 * 1024)

static char child_stack[STACK_SIZE];

char *const child_args[] = {"/bin/bash", NULL};

int child_main(void *args)
{
    printf("child process: %d\n", getpid());
    sethostname("test_child_host", 12);
    execv(child_args[0], child_args);
    return 1;
}

int main()
{
    printf("main process: %d\n", getpid());
    int child_pid = clone(child_main, child_stack + STACK_SIZE, CLONE_NEWUTS | SIGCHLD, NULL);
    waitpid(child_pid, NULL, 0);
    printf("exit.\n");
    return 0;
}