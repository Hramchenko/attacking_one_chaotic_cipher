#ifndef __HGAMMA_ANALYZER_H__
#define __HGAMMA_ANALYZER_H__

#include "hopencl_program.h"

struct _hgamma_analyzer{
  hopencl_program_t* opencl_program;
  float* result_values;
  size_t result_block_size;
  size_t out_block_size;
  unsigned char map_number;
  cl_mem mem_input;
  cl_mem mem_output;
  cl_int error;
};

typedef struct _hgamma_analyzer hgamma_analyzer_t;

extern hgamma_analyzer_t* hgamma_analyzer_create(size_t input_block_size, unsigned char map_number);
extern void hgamma_analyzer_init(hgamma_analyzer_t* logistic);
extern void hgamma_analyzer_process(hgamma_analyzer_t* analyzer, float* initial_values, size_t initial_count, unsigned char next_gamma, unsigned int out_block_size);
extern void hgamma_analyzer_exec(char* input, char* output, unsigned char next_gamma, unsigned char map_number);
extern void hgamma_analyzer_free(hgamma_analyzer_t* logistic);

#endif
