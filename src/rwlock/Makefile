CC = g++
CFLAGS = -g -Wall -Werror
LDFLAGS = -pthread

BINARIES = RWLockTest

all: $(BINARIES)

RWLockTest: RWLock.cpp RWLock.h RWLockTest.cpp
	$(CC) $(LDFLAGS) $(CFLAGS) RWLockTest.cpp RWLock.cpp -o RWLockTest

clean:
	rm -f *.o
	rm -f $(BINARIES)
