#ifndef _H_OPENCL_PROGRAM_H_
#define _H_OPENCL_PROGRAM_H_

#include <CL/cl.h>

#include "hopencl_functions.h"

struct _hopencl_program{
  cl_int error;
  cl_platform_id platform;
  cl_device_id device;
  cl_context context;
  cl_command_queue command_queue;
  cl_program program;
  cl_kernel kernel;
};

typedef struct _hopencl_program hopencl_program_t;


extern hopencl_program_t* hopencl_program_create();
extern void hopencl_program_init_device(hopencl_program_t* program, cl_uint platform, cl_uint device);
extern void hopencl_program_load(hopencl_program_t* program, char* file_name, char* build_options, char* kernel_name);
extern void hopencl_program_start_kernel(hopencl_program_t* program, cl_uint dimension, size_t* data_dimensions, size_t* work_group_dimensions);
extern void hopencl_program_finish(hopencl_program_t* program);
extern void hopencl_program_free(hopencl_program_t* program);
extern void hopencl_program_create_buffer(hopencl_program_t* program, cl_mem* target, size_t buffer_size, cl_mem_flags flags, cl_uint arg_index);
extern void hopencl_set_kernel_argument(hopencl_program_t* program, const void* arg_value, size_t arg_size, cl_uint arg_index);
extern void hopencl_program_free_buffer(cl_mem buffer);
extern void hopencl_program_read_from_buffer(hopencl_program_t* program, cl_mem buffer, void* data, size_t data_size);
extern void hopencl_program_write_to_buffer(hopencl_program_t* program, cl_mem buffer, void* data, size_t data_size);

#endif
