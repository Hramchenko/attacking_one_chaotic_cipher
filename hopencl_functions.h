#ifndef __HOPENCL_FUNCTIONS_H__
#define __HOPENCL_FUNCTIONS_H__

#include <stdlib.h>
#include <stdio.h>
#include <CL/cl.h>

extern char* read_code_from_file(const char* filename, size_t* size);
extern void print_opencl_info();
extern void print_platform_info(cl_platform_id platform);
extern void print_device_info(cl_device_id id);
extern cl_platform_id get_n_platform(int n, cl_int* error);
extern cl_device_id get_n_device(cl_platform_id platform, int n, cl_int* error);
extern cl_program load_program_from_file(cl_device_id device, cl_context context, char* filename, char* build_options, cl_int* error);
extern void print_opencl_error(cl_int error);
extern FILE* fopen_and_check(char* filename, char* mode);


#endif
