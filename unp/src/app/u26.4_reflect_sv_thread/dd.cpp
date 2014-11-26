#include <stdio.h>
#include <pthread.h>

int count = 0;
pthread_mutex_t count_mutex = PTHREAD_MUTEX_INITIALIZER;


void *thread_main(void *ptr)
{
    for (int i=0; i<100000; i++) {
        pthread_mutex_lock(&count_mutex);
        int temp = count;
        printf("%d：%d\n", pthread_self(), temp + 1);
        count = temp + 1;
        pthread_mutex_unlock(&count_mutex);
    }
    return NULL;
}

int main(int argc, char **argv)
{
    pthread_t tid1, tid2;
    pthread_create(&tid1, NULL, &thread_main, NULL);
    pthread_create(&tid2, NULL, &thread_main, NULL);
    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);
    return 0;
}
