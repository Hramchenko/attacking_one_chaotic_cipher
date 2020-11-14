#ifndef __HCIPHER_ALL_H__
#define __HCIPHER_ALL_H__

#include "hopencl_program.h"

struct _hcipher_all{
  hopencl_program_t* opencl_program;
  float* result_x;
  unsigned char* result_gamma;
  unsigned char map_number;
  cl_mem x_output;
  cl_mem gamma_output;
  cl_int error;
};

typedef struct _hcipher_all hcipher_all_t;

extern void hcipher_all_exec(char* start, char* output);
extern hcipher_all_t* hcipher_all_create(unsigned char map_number);
extern void hcipher_all_process(hcipher_all_t* cipher, float start_x);
extern void hcipher_all_free(hcipher_all_t* cipher);
extern void hcipher_all_init(hcipher_all_t* cipher);

#endif
