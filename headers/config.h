#ifndef __CONFIG_H__
#define __CONFIG_H__

typedef int Elem_t;
const Elem_t POISON = -31415926;
#define OUTPUT_F "%d"

#define CANARY_PROTECTION

#define HASH_PROTECTION

const size_t INIT_CAPACITY     = 4;
const size_t RESIZE_MULTIPLIER = 2;

#endif
