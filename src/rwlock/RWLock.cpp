#include "RWLock.h"

pthread_mutex_t read_write_mutex;
pthread_cond_t turn_cv;
unsigned int read_count = 0;
unsigned int writing_count = 0;
bool writing = false;

RWLock :: RWLock() {
	pthread_cond_init(&turn_cv, NULL);
}

void RWLock :: rlock() {
	pthread_mutex_lock(&read_write_mutex);
	if(writing_count > 0){
		pthread_cond_wait(&turn_cv, &read_write_mutex);
	}
	while(writing){
		pthread_cond_wait(&turn_cv, &read_write_mutex);
	}
	read_count++;
	pthread_mutex_unlock(&read_write_mutex);
}

void RWLock :: wlock() {
	pthread_mutex_lock(&read_write_mutex);
	writing_count++;
	while(writing || read_count > 0){
		pthread_cond_wait(&turn_cv, &read_write_mutex);
	}
	writing = true;
	pthread_mutex_unlock(&read_write_mutex);
}

void RWLock :: runlock() {
	pthread_mutex_lock(&read_write_mutex);
	read_count--;
	if(read_count == 0){
		pthread_cond_broadcast(&turn_cv);
	}
	pthread_mutex_unlock(&read_write_mutex);
}

void RWLock :: wunlock() {
	pthread_mutex_lock(&read_write_mutex);
	writing = false;
	writing_count--;
	pthread_cond_broadcast(&turn_cv);
	pthread_mutex_unlock(&read_write_mutex);
}
