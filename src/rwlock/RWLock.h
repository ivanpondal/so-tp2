#ifndef RWLock_h
#define RWLock_h
#include <pthread.h>

class RWLock {
    public:
        RWLock();
        void rlock();
        void wlock();
        void runlock();
        void wunlock();
};

#endif
