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
#include <time.h>

#define SHM_SIZE_1M 0x100000 /* 1M */
#define SHM_SIZE_1G 0x40000000 /* 1G */
#define SHM_SIZE_4G 0x100000000 /* 4G */
#define SHM_SIZE_8G 0x200000000 /* 8G */
#define SHM_START1 0x7ffff6cfc000 /**/
#define SHM_START2 0x7fffb8dfc000 /* 0x7fffb7dfc000 will restore to 0x7fffb7dfc000 */
#define SHM_START3 0x7fffb6dfc000 /* 0x7fffb7dfc000 will restore to 0x7fffb7dfc000 */
#define SHM_START4 0x7ffdf7dfc000 /* 8G */

#define SHM_SIZE SHM_SIZE_4G
#define SHM_START SHM_START4

#ifdef __x86_64__
#define SYSCALL_PCN_DSHM_MMAP   334
#define SYSCALL_PCN_DSHM_MUNMAP   335
#define SYSCALL_GETTID 186
#elif __aarch64__
#define SYSCALL_PCN_DSHM_MMAP   289
#define SYSCALL_PCN_DSHM_MUNMAP   290
#define SYSCALL_GETTID 178
#elif __powerpc64__
#define SYSCALL_PCN_DSHM_MMAP   -1
#define SYSCALL_PCN_DSHM_MUNMAP   -2
#define SYSCALL_GETTID 207
#else
#error Does not support this architecture
#endif

long int popcorn_dshm_mmap(unsigned long addr, unsigned long len,
					unsigned long prot, unsigned long flags,
					unsigned long fd, unsigned long pgoff) {
	return syscall(SYSCALL_PCN_DSHM_MMAP, addr, len, prot, flags, fd, pgoff);
}

long int popcorn_dshm_munmap(unsigned long addr, size_t len) {
	return syscall(SYSCALL_PCN_DSHM_MUNMAP, addr, len);
}

int main()
{
    //unsigned long i;
    char *map = NULL;

    printf("This is parent process. Request addr 0x%lx size 0x%lx = %ldM\n",
			(unsigned long)SHM_START, (unsigned long)SHM_SIZE,
			(unsigned long)SHM_SIZE / 1024 / 1024);
    //map = mmap(NULL, SHM_SIZE_1G, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    //map = mmap((void*)SHM_START, SHM_SIZE_1G, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    map = (char*)popcorn_dshm_mmap(SHM_START, SHM_SIZE,
				PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
	if (map) {
		printf("mmap *ptr = %p size = 0x%lx (sizeof map 0x%lx)\n",
				map, (unsigned long)SHM_SIZE, sizeof(map));
	} else {
		printf("ERROR: Got a big problem: NULL returned mmap *ptr\n");
		exit(-1);
	}

	if (map != (void*)SHM_START) {
		printf("ERROR: Got a mmap *ptr that is not I want (want: %p got: %p)\n",
					(void*)SHM_START, map);
		if (map) {
			printf("popcorn_dshm_munmap()\n");
			popcorn_dshm_munmap((unsigned long)map, SHM_SIZE);
		}
		printf("ERROR: doesn't get expected returned mmap *ptr\n");
		exit(-1);
	}
    printf("\nALLOC ***GOOD***:got map %p == (expect 0x%lx)\n\n\n",
			map, (unsigned long)SHM_START);
	sleep(3);

    /* Sync */
	printf("sync bit init (sleep5)\n");
	*map = 0; // W // works
	sleep(5);
	printf("sync wait\n");
	while (*map != 0) { // R
		printf("[!0 wait] *map = %c %d\n", *map, *map); // R
		sleep(1);
	}
	// *map == 0
    *map = 1; // W
	printf("[1 notify] *map = %c %d\n", *map, *map); // R


    printf("all done. sleep 5s\n");
    sleep(5);
    printf("skip munmap...\n");
	//if (map) {
	//	printf("popcorn_dshm_munmap()\n");
	//	popcorn_dshm_munmap((unsigned long)map, SHM_SIZE);
	//	map = NULL;
	//}


	printf("sleep 10s and exit\n");
	sleep(10);
	printf("exit\n");
    return 0;
}
