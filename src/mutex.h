#pragma once
#include <stdint.h>
#include "i386.h"

typedef struct
{
    uint32_t ass;
} mutex_t;

mutex_t mutex_init();
void mutex_lock(mutex_t *mutex);
void mutex_unlock(mutex_t *mutex);

