#include "RWLock.h"
#include <iostream>

#define NUM_THREADS 20

RWLock counter_lock;
int counter = 0;
pthread_t thread[NUM_THREADS];
int counter_array[NUM_THREADS];

void* reader(void*){
	counter_lock.rlock();
	std::cout << "LEYENDO counter = " << counter << std::endl;
	counter_lock.runlock();

	return NULL;
}

void* writer(void*){
	counter_lock.wlock();
	counter_array[counter] = counter;
	counter++;
	counter_lock.wunlock();

	return NULL;
}

int main(){
	for(int i = 0; i < NUM_THREADS; i++){
		if(i % 2 == 0){
			pthread_create(&thread[i], NULL, reader, NULL);
		}
		else{
			pthread_create(&thread[i], NULL, writer, NULL);
		}
	}
	for(int i = 0; i < NUM_THREADS; i++){
		pthread_join(thread[i], NULL);
	}

	bool no_error = true;
	for(int i = 0; i < NUM_THREADS/2; i++){
		if(counter_array[i] != i){
			std::cout << "ERROR counter[" << i << "] != " << i << std::endl;
			no_error = false;
		}
	}

	if(no_error){
			std::cout << "La prueba terminó correctamente" << std::endl;
	}
}
