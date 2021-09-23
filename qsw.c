/*
 * qsw.c
 * Copyright (C) 2021 jackchuang <jackchuang@echo5>
 *
 * Distributed under terms of the MIT license.
 */

#include <sys/mman.h>
#include <unistd.h>
#include <fcntl.h>       /* For O_* constants */
#include <string.h>
#include <stdio.h>

//int main(int argc, char **argv)
//{
//    int fd = shm_open("posixsm", O_CREAT | O_RDWR, 0666);
//    ftruncate(fd, 0x400000);
//    
//    char *p = mmap(NULL, 0x400000,
//                   PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
//    memset(p, 'A', 0x400000);
//    munmap(p, 0x400000);
//    
//    return 0;
//}

#define SHM_SIZE 0x100000

int main(int argc, char **argv)
{
    // int fd = shm_open("posixsm", O_CREAT | O_RDWR, 0666);
	int fd = open("/dev/shm/share", O_CREAT | O_RDWR);
	printf("fd = %d\n", fd);
    ftruncate(fd, SHM_SIZE);
    char *p = mmap(NULL, SHM_SIZE,
                   PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

	printf("p = %p\n", p);
	memset(p, 'A', SHM_SIZE);
    munmap(p, SHM_SIZE);
    return 0;
}
