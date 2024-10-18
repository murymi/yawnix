#include<stdint.h>

//static inline uint32_t 
#define align_forward(n, target) (((n) + (target) - 1) & ~((target) - 1))

#define align_backward(n, target)  ((n)  & ~((target) - 1))