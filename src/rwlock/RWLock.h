#ifndef RWLock_h
#define RWLock_h
#include <iostream>
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <queue>          // std::stack

using namespace std;

class RWLock {
    public:
        RWLock();
        void rlock();
        void wlock();
        void runlock();
        void wunlock();
    private:
    	pthread_mutex_t queue_mutex; 
    	queue< bool > request_queue; // true = write, false = read 
    	queue< pair < pthread_cond_t,pthread_mutex_t > > read_queue;
    	queue<pthread_mutex_t> write_queue;
    	pthread_mutex_t cond_mutex;
    	bool write_flag;
    	int read_count;

};

#endif
