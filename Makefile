#
# Makefile
# jackchuang, 2020-09-16 14:20
#
CC = $(CROSS_COMPILE)gcc
OBJDUMP = $(CROSS_COMPILE)objdump -d -S

LIBMIGRATE = libmigrate.a
LIBSHM = -lrt
POPCORN_INC_PATH = -I../include

CFLAGS += -O2
CFLAGS += -DDEBUG -g -Wall $(POPCORN_INC_PATH)
#CFLAGS += -DWAIT_FOR_DEBUGGER



#LDFLAGS += -static -L$(POPCORN_LIB_PATH)
#LIBS += -pthread -l:$(LIBMIGRATE)
#LIBS += -pthread -l:$(LIBMIGRATE) $(LIBSHM) # wrong
LIBS = $(LIBSHM)

TARGETS = sr sw psr psw qsr qsw
OBJDUMPS =

all: $(TARGETS) $(OBJDUMPS)

%.o: %.c
	$(CC) -c $(CFLAGS) $^ $(LIBS) -o $@

sr: sr.o
	$(CC) $(LDFLAGS) $^ $(LIBS) -o $@
sw: sw.o
	$(CC) $(LDFLAGS) $^ $(LIBS) -o $@

psr: psr.o
	$(CC) $(LDFLAGS) $^ $(LIBS) -o $@
psw: psw.o
	$(CC) $(LDFLAGS) $^ $(LIBS) -o $@

qsr: qsr.o
	$(CC) $(LDFLAGS) $^ $(LIBS) -o $@
qsw: qsw.o
	$(CC) $(LDFLAGS) $^ $(LIBS) -o $@

#$(TARGETS): $(TARGETS).o
#	$(CC) $(LDFLAGS) $^ $(LIBS) -o $@

clean:
	rm -f $(TARGETS) $(OBJDUMPS) *.o
