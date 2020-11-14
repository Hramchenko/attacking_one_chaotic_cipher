#ifndef __hinitial_ic_generator_H__
#define __hinitial_ic_generator_H__

#include "hopencl_program.h"
#include "hkey_ic_generator.h"
#include "hvalues_writer.h"

struct _hinitial_ic_generator{
  hopencl_program_t* opencl_program;
  hvalues_writer_t* writer;
  hkey_ic_generator_t* key_generator;
  float* result_values;
  unsigned int values_count;
  unsigned char map_number;
  cl_mem mem_input;
  cl_mem mem_output;
  cl_int error;
};

typedef struct _hinitial_ic_generator hinitial_ic_generator_t;

extern void hinitial_ic_generator_exec();

#endif
