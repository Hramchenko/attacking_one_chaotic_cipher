#include "hkey_ic_generator.h"

#include <stdio.h>
#include <math.h>

#include "hinitial_ic_generator.h"
#include "hglobal_constants.h"

void hkey_ic_generator_init_basic_parameters(hkey_ic_generator_t* generator);

hkey_ic_generator_t* hkey_ic_generator_create(){
  hkey_ic_generator_t* result = malloc(sizeof(hkey_ic_generator_t));
  result->keys_block = malloc(MAX_IC_COUNT*sizeof(float));
  result->keys_count = MAX_IC_COUNT;
  return result;
}

void hkey_ic_generator_generate(hkey_ic_generator_t* generator){
  unsigned int index;
  double value;
  for (index = 0; index <  generator->keys_count; index++){
    value = ((double)index)/IC_DIVIDER;
    value = value - floor(value);
    generator->keys_block[index] = value;
    //printf("%f\n", generator->keys_block[index]);
  }
}

void hkey_ic_generator_free(hkey_ic_generator_t* generator){
  free(generator->keys_block);
  free(generator);
}
