#include "RWLock.h"


RWLock :: RWLock() {
	read_count = 0;
	write_flag = false;
}

void RWLock :: rlock() {
	bool local_write_flag;
	pthread_mutex_lock(&cond_mutex);
		local_write_flag = write_flag;
	pthread_mutex_unlock(&cond_mutex);
	if (!local_write_flag){
		pthread_mutex_lock(&cond_mutex);
			read_count ++;
		pthread_mutex_unlock(&cond_mutex);
	}else{

		pthread_mutex_lock(&queue_mutex);

			if (request_queue.front()){ // si lo último fue escritura 
				//Creo la nueva condición para los nuevos reads y la encolo
				pthread_cond_t cond; 
				pthread_cond_init(&cond, NULL);
				pthread_mutex_t mutex;
				pair < pthread_cond_t, pthread_mutex_t > new_read = make_pair(cond,mutex);
				read_queue.push(new_read);
			}

			
			// Espero a que terminen las últimas lecturas pendientes
			pair < pthread_cond_t, pthread_mutex_t > first_read = read_queue.front();
			pthread_mutex_lock(&first_read.second);
				while (local_write_flag) {
			    	pthread_cond_wait(&first_read.first, &first_read.second);
			    	pthread_mutex_lock(&cond_mutex);
						local_write_flag = write_flag;
					pthread_mutex_unlock(&cond_mutex);
				}

		    pthread_mutex_unlock(&first_read.second);

		(&queue_mutex);
	}
}

void RWLock :: wlock() {
	int local_read_count;
	bool local_write_flag;
	pthread_mutex_lock(&cond_mutex);
		local_write_flag = write_flag;
		local_read_count = read_count;
	pthread_mutex_unlock(&cond_mutex);
	if (!local_write_flag && local_read_count == 0){// si no hay nada para leer y no hay nadie escribiendo
		pthread_mutex_lock(&cond_mutex);
		write_flag = true;
		pthread_mutex_unlock(&cond_mutex);
	}else{

		pthread_mutex_t new_write;
		pthread_mutex_lock(&queue_mutex);
			
			write_queue.push(new_write);
			pthread_mutex_lock(&new_write);
			pthread_mutex_lock(&cond_mutex);
				write_flag = true;
			pthread_mutex_unlock(&cond_mutex);

		pthread_mutex_unlock(&queue_mutex);
	}
}

void RWLock :: runlock() {
	int local_read_count;
	pthread_mutex_lock(&cond_mutex);
		read_count --;
		local_read_count = read_count;
	pthread_mutex_unlock(&cond_mutex);
	if (local_read_count == 0){ // si no hay lecturas pendientes
		pthread_mutex_lock(&queue_mutex);
			
			if (!write_queue.empty()){ // si tengo alguna escritura pendiente
				request_queue.pop();
				pthread_mutex_t next_write = write_queue.front();
				write_queue.pop();
				pthread_mutex_unlock(&next_write);
			}

		pthread_mutex_unlock(&queue_mutex);
		
	}
}

void RWLock :: wunlock() {
	pthread_mutex_lock(&cond_mutex);
		write_flag = false;
	pthread_mutex_unlock(&cond_mutex);

	bool is_empty;
	pthread_mutex_lock(&queue_mutex);
		is_empty = request_queue.empty();
	pthread_mutex_unlock(&queue_mutex);

	if (!is_empty){
		bool next_operation;
		pthread_mutex_lock(&queue_mutex);
			next_operation = request_queue.front();
			request_queue.pop();
		pthread_mutex_unlock(&queue_mutex);
		if (next_operation){
			//libero el proximo write
			pthread_mutex_lock(&queue_mutex);
				pthread_mutex_t next_write_mutex =  write_queue.front();
				write_queue.pop();
				pthread_mutex_unlock(&next_write_mutex);
			pthread_mutex_unlock(&queue_mutex);
		}else{
			//libero el/los proximo/s read/s
			pthread_mutex_lock(&queue_mutex);
				pthread_cond_t next_read_cond = read_queue.front().first;
				read_queue.pop();
				pthread_cond_broadcast(&next_read_cond);
			pthread_mutex_unlock(&queue_mutex);
		}

	}

}
