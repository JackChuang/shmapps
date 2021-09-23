/*
 * private-anonymous-mapping.c
 * Copyright (C) 2021 jackchuang <jackchuang@echo5>
 *
 * Distributed under terms of the MIT license.
 */
#include <sys/mman.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define SHM_SIZE 0x100000 /* 1M */

int main()
{
    int pid;
    char *map;

    printf("parent process...\n");
    //map = mmap(NULL, SHM_SIZE, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    map = mmap(NULL, SHM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    printf("%s\n", map);

    map[0] = 'h';
    map[1] = 'l';

    if((pid = fork()) == -1) {
        perror("fork");
    }

    if(pid == 0) { /* child writes first*/
        printf("child process...\n");
        printf("%s\n", map);

        map[2] = 'l';
        getchar();
        exit(0);
    }

    if(pid > 0) { /* parent reads after */
        waitpid(pid, NULL, 0);
        printf("%s\n", map);
    }

    return 0;
}
