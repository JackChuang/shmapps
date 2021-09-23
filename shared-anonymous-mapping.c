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

#define SHM_SIZE_1M 0x100000 /* 1M */
#define SHM_SIZE_1G 0x40000000 /* 1G */
#define SHM_START1 0x7ffff6cfc000 /**/
#define SHM_START2 0x7fffb8dfc000 /* 0x7fffb7dfc000 will restore to 0x7fffb7dfc000 */
#define SHM_START3 0x7fffb6dfc000 /* 0x7fffb7dfc000 will restore to 0x7fffb7dfc000 */

#define SHM_START SHM_START3
int main()
{
    int pid;
    char *map = NULL;

    printf("This is parent process.\n");
    //map = mmap(NULL, SHM_SIZE_1G, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    map = mmap((void*)SHM_START, SHM_SIZE_1G, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
	if (map)
		printf("mmap *ptr = %p size = 0x%x (sizeof map 0x%lx)\n", map, SHM_SIZE_1G, sizeof(map));
	else {
		printf("Got a big problem: NULL returned mmap *ptr\n");
		exit(-1);
	}

	if (map != (void*)SHM_START) {
		printf("Got a mmap *ptr that is not I want (want: %p got: %p)\n",
					(void*)SHM_START, map);
		if (map) {
			munmap(map, SHM_SIZE_1G);
		}
		exit(-1);
	}

    map[0] = 'h';
    map[1] = 'l';
    printf("*mmap = %s\n", map);

    if((pid = fork()) == -1) {
        perror("fork");
    }

    if(pid == 0) { /* child writes first*/
        printf("[child] This is child process.\n");
        printf("[child] *mmap = %s\n", map);

        map[2] = 'l';
        printf("[child] *mmap = %s\n", map);
        //getchar(); // pause
		if (map)
			munmap(map, SHM_SIZE_1G);
        exit(0);
    }

    if(pid > 0) { /* parent reads after */
        waitpid(pid, NULL, 0);
        printf("*mmap %s\n", map);
    }

	if (map)
		munmap(map, SHM_SIZE_1G);
    return 0;
}

/*
This is parent process.
mmap *ptr = 0x7fffb6dfc000 size = 0x40000000 (sizeof map 0x8)
*mmap = hl
[child] This is child process.
[child] *mmap = hl
[child] *mmap = hll
*mmap hll
*/
