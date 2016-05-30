#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

pthread_cond_t cv;
pthread_mutex_t mutex;
bool cond;

void* print1(void*) {
    pthread_mutex_lock(&mutex);
    while (! cond) {
        pthread_cond_wait(&cv, &mutex);
    }
    pthread_mutex_unlock(&mutex);
    printf("1\n");
}

void* print2(void*) {
    pthread_mutex_lock(&mutex);
    while (! cond) {
        pthread_cond_wait(&cv, &mutex);
    }
    pthread_mutex_unlock(&mutex);
    printf("2\n");
}

int main(int argc, char **argv) {
    cond = false;
    pthread_cond_init(&cv, NULL);
    // pthread_mutex_init()

    pthread_t thread[2]; int tid;

    pthread_create(&thread[0], NULL, print1, NULL);
    pthread_create(&thread[1], NULL, print2, NULL);

    sleep(2);
    pthread_mutex_lock(&mutex);
        cond = true;
        pthread_cond_broadcast(&cv);
    pthread_mutex_unlock(&mutex);

    pthread_join(thread[0], NULL);
    pthread_join(thread[1], NULL);

    return 0;
}
