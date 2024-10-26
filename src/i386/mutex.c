#include"mutex.h"

void mutex_lock(mutex_t *mutex) {
    while (compare_and_swap(&mutex->ass, 0, 1) != 0){}
}

void mutex_unlock(mutex_t *mutex) {
    while (compare_and_swap(&mutex->ass, 1, 0) != 1){}
}

mutex_t mutex_init() {
    return (mutex_t){.ass = 0};
}