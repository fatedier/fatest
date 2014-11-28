#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define MAX_PRODUCT 5000000
#define THREADS_NUM 5

typedef struct
{
    pthread_mutex_t mutex;
    int buf[MAX_PRODUCT];
    int nput;
    int next_val;
} share_t;

share_t share_data = {PTHREAD_MUTEX_INITIALIZER};

void *produce(void *ptr)
{
    for (;;) {
        pthread_mutex_lock(&share_data.mutex);
        if (share_data.next_val >= MAX_PRODUCT) {
            pthread_mutex_unlock(&share_data.mutex);
            return NULL;
        }

        share_data.buf[share_data.next_val] = share_data.nput;
        share_data.next_val++;
        share_data.nput++;
        pthread_mutex_unlock(&share_data.mutex);

        *(int *)ptr += 1;
    }
}

void *consume(void *ptr)
{
    for (int i=0; i<THREADS_NUM; i++) {
        if (share_data.buf[i] != i)
            printf("error: buf[%d] = %d\n", i, share_data.buf[i]);
    }
    return NULL;
}

int main(int argc, char **argv)
{
    share_data.nput = 0;
    share_data.next_val = 0;

    int count[THREADS_NUM];
    for (int i=0; i<THREADS_NUM; i++)
        count[i] = 0;

    pthread_t tid_p[THREADS_NUM];
    for (int i=0; i<THREADS_NUM; i++) {
        pthread_create(&tid_p[i], NULL, produce, &count[i]);
    }

    for (int i=0; i<THREADS_NUM; i++) {
        pthread_join(tid_p[i], NULL);
        printf("thread %d count is %d\n", i, count[i]);
    }

    pthread_t tid_c;
    pthread_create(&tid_c, NULL, consume, NULL);
    pthread_join(tid_c, NULL);

    return 0;
}
