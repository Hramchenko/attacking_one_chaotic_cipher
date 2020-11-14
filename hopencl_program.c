#include "hopencl_program.h"

#include <stdlib.h>
#include <stdio.h>

#include "hopencl_functions.h"

hopencl_program_t* hopencl_program_create(){
  hopencl_program_t* result = malloc(sizeof(hopencl_program_t));
  return result;
}

void hopencl_program_init_device(hopencl_program_t* program, cl_uint platform, cl_uint device){
  program->platform = get_n_platform(platform, &program->error);
  if (program->error){
    print_opencl_error(program->error);
    printf("Can't get platform id.\n");
  }
  program->device = get_n_device(program->platform, device, &program->error);
  if (program->error){
    print_opencl_error(program->error);
    printf("Can't get device id.\n");
  }
  program->context = clCreateContext(0, 1, &program->device, 0, 0, &program->error);
  if (program->error){
    print_opencl_error(program->error);
    printf("Can't create context.\n");
  }
  program->command_queue = clCreateCommandQueue(program->context, program->device, 0, &program->error);
  if (program->error){
    print_opencl_error(program->error);
    printf("Can't create command queue.\n");
  }
}

void hopencl_program_load(hopencl_program_t* program, char* file_name, char* build_options, char* kernel_name){
  program->program = load_program_from_file(program->device, program->context, file_name, build_options, &program->error);
  if (program->error){
    print_opencl_error(program->error);
    printf("Can't create program.\n");
  }
  program->kernel = clCreateKernel(program->program, kernel_name, &program->error);
  if (program->error){
    print_opencl_error(program->error);
    printf("Can't create kernel.\n");
  }
}

void hopencl_program_start_kernel(hopencl_program_t* program, cl_uint dimension, size_t* data_dimensions, size_t* work_group_dimensions){
  program->error = clEnqueueNDRangeKernel(program->command_queue, program->kernel, dimension, 0, data_dimensions, work_group_dimensions, 0, 0, 0);
  if (program->error){
    print_opencl_error(program->error);
    printf("Can't start kernel.\n");
  }
}

void hopencl_program_finish(hopencl_program_t* program){

  program->error = clFinish(program->command_queue);
  fflush(stdout);
  if (program->error){
    print_opencl_error(program->error);
    printf("Can't finish program.\n");
  }
}

void hopencl_set_kernel_argument(hopencl_program_t* program, const void* arg_value, size_t arg_size, cl_uint arg_index){
  program->error = clSetKernelArg(program->kernel, arg_index, arg_size, arg_value);
  if (program->error){
    print_opencl_error(program->error);
    printf("Can't set kernel arg.\n");
  }
}

void hopencl_program_create_buffer(hopencl_program_t* program, cl_mem* target, size_t buffer_size, cl_mem_flags flags, cl_uint arg_index){
  *target = clCreateBuffer(program->context, flags, buffer_size, 0, &program->error);
  if (program->error){
    print_opencl_error(program->error);
    printf("Can't allocate buffer.\n");
  }
  hopencl_set_kernel_argument(program, target, sizeof(cl_mem), arg_index);
}

void hopencl_program_free_buffer(cl_mem buffer){
  clReleaseMemObject(buffer);
}

void hopencl_program_read_from_buffer(hopencl_program_t* program, cl_mem buffer, void* data, size_t data_size){
  program->error = clEnqueueReadBuffer(program->command_queue, buffer, CL_FALSE, 0, data_size, data, 0, 0, 0);
  if (program->error){
    print_opencl_error(program->error);
    printf("Can't read buffer.\n");
  }
}

void hopencl_program_write_to_buffer(hopencl_program_t* program, cl_mem buffer, void* data, size_t data_size){
  program->error = clEnqueueWriteBuffer(program->command_queue, buffer, CL_FALSE, 0, data_size, data, 0, 0, 0);
  if (program->error){
    print_opencl_error(program->error);
    printf("Can't write buffer.\n");
  }
}

void hopencl_program_free(hopencl_program_t* program){
  clReleaseKernel(program->kernel);
  clReleaseProgram(program->program);
  clReleaseCommandQueue(program->command_queue);
  clReleaseContext(program->context);
  free(program);
}

