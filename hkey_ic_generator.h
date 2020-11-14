#ifndef __HKEY_GENERATOR_H__
#define __HKEY_GENERATOR_H__

#include <stdlib.h>

struct _hkey_ic_generator{
  size_t keys_count;
  float* keys_block;
};

typedef struct _hkey_ic_generator hkey_ic_generator_t;

extern hkey_ic_generator_t* hkey_ic_generator_create();
extern void hkey_ic_generator_generate(hkey_ic_generator_t* generator);
extern void hkey_ic_generator_free(hkey_ic_generator_t* program);


#endif
