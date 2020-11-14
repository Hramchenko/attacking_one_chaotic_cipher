#ifndef __hlogistic_generator_H__
#define __hlogistic_generator_H__

#include "hopencl_program.h"

struct _hlogistic_generator{
  hopencl_program_t* opencl_program;
  float x_0;
  unsigned char* key;
  unsigned char* generators;
  unsigned int key_length;
  int values_count;
  float* x_values;
  unsigned char* gamma_values;
  cl_mem mem_key_input;
  cl_mem mem_generators_input;
  cl_mem mem_gamma_output;
  cl_mem mem_x_output;
  cl_int error;
};

typedef struct _hlogistic_generator hlogistic_generator_t;

hlogistic_generator_t* hlogistic_generator_create(float x_0, unsigned char* key,  unsigned char* generators, unsigned int key_length, float* x_values, unsigned char* gamma_values, int values_count);
void hlogistic_generator_init(hlogistic_generator_t* logistic);
void hlogistic_generator_process(hlogistic_generator_t* logistic);
void hlogistic_generator_print_result(hlogistic_generator_t* logistic);
void hlogistic_generator_free(hlogistic_generator_t* logistic);
void hlogistic_generator_exec(char* input, char* output);

#endif
