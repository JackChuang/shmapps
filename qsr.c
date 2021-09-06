/*
 * qsr.c
 * Copyright (C) 2021 jackchuang <jackchuang@echo5>
 *
 * Distributed under terms of the MIT license.
 */

#include <sys/mman.h>
#include <unistd.h>
#include <fcntl.h>       /* For O_* constants */
#include <stdlib.h>
#include <stdio.h>

//int main(int argc, char **argv)
//{
//    int fd = shm_open("posixsm", O_RDONLY, 0666);
//    ftruncate(fd, 0x400000);
//    
//    char *p = mmap(NULL, 0x400000,
//                   PROT_READ, MAP_SHARED, fd, 0);  
//    printf("%c %c %c %c\n", p[0], p[1], p[2], p[3]);
//    
//    munmap(p, 0x400000);
//    
//    return 0;
//}

#define SHM_SIZE 0x100000

int main(int argc, char **argv)
{
    //int fd = shm_open("posixsm", O_RDONLY, 0666);
    int fd = open("/dev/shm/share", O_RDWR);
	ftruncate(fd, SHM_SIZE);
    char *p = mmap(NULL, SHM_SIZE,
                   PROT_READ, MAP_SHARED, fd, 0);  
    printf("firt %c %c %c %c ~ last %c %c %c %c size %d\n",
			p[0], p[1], p[2], p[3],
			p[SHM_SIZE - 4], p[SHM_SIZE - 3],
			p[SHM_SIZE - 2], p[SHM_SIZE - 1], SHM_SIZE);
    munmap(p, SHM_SIZE);
    return 0;
}

