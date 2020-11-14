#include "hopencl_functions.h"

#include <stdio.h>

char* read_code_from_file(const char* filename, size_t* size){
  char* mem = 0;
  int mem_size = 0;
  FILE *in;
  in = fopen(filename, "rb");
  if (!in){
    *size = 0;
    return 0;
  }
  fseek(in, 0, 2);
  mem_size = ftell(in);
  fseek(in, 0, 0);
  mem = malloc((mem_size + 1)*sizeof(char));
  *size = fread(mem, 1, mem_size, in);
  mem[mem_size] = 0;
  fclose(in);
  return mem;
}

cl_uint platforms_count(){
  cl_uint count;
  clGetPlatformIDs (0, NULL, &count);
  return count;
}


void print_opencl_info(){
  cl_uint num_platforms = platforms_count();
  printf("---------------------------------------\n");
  printf("Platforms count: %d\n", num_platforms);
  cl_uint i;
  cl_platform_id platform_ids[num_platforms];
  clGetPlatformIDs (num_platforms, platform_ids, 0);
  for (i = 0; i < num_platforms; i++){
    printf("---------------------------------------\n");
    printf("Platform #%d:\n", i);
    print_platform_info(platform_ids[i]);
  }
  printf("---------------------------------------\n");
}

void print_platform_info(cl_platform_id platform){
  int name_size = 1024;
  char platform_name[name_size];
  clGetPlatformInfo(platform, CL_PLATFORM_NAME, name_size, platform_name, NULL);
  printf("Platform name: %s\n", platform_name);
  cl_uint max_devices_count = 10;
  cl_device_id devices[max_devices_count];
  cl_uint devices_count;
  clGetDeviceIDs(platform, CL_DEVICE_TYPE_ALL, max_devices_count, devices, &devices_count);
  cl_uint i;
  for (i = 0; i < devices_count; i++){
    printf("Device #%d\n", i);
    print_device_info(devices[i]);
  }
}

void print_device_info(cl_device_id id){
  int name_size = 1024;
  char device_name[name_size];
  clGetDeviceInfo(id, CL_DEVICE_NAME, name_size, &device_name, 0);
  printf("Device name: %s\n", device_name);
}

cl_platform_id get_n_platform(int n, cl_int* error){
  cl_platform_id platform_ids[n + 1];
  *error = clGetPlatformIDs(n + 1, platform_ids, 0);
  return platform_ids[n];
}

cl_device_id get_n_device(cl_platform_id platform, int n, cl_int* error){
  cl_device_id devices[n + 1];
  *error = clGetDeviceIDs(platform, CL_DEVICE_TYPE_ALL, n + 1, devices, 0);
  return devices[n];
}


cl_program load_program_from_file(cl_device_id device, cl_context context, char* filename, char* build_options, cl_int* error){
  size_t program_size;
  char *src = read_code_from_file(filename, &program_size);
  if(!src){
    *error = 404;
    return 0;
  }
  cl_program prog = clCreateProgramWithSource(context, 1, (const char**)&src, &program_size, error);
  print_opencl_error(*error);
  *error = clBuildProgram(prog, 0, 0, build_options, 0, 0);
  if (*error){
    printf("%s\n" , src);
    printf("OpenCL program building error %d:\n", (int)*error);
    char *build_log;
    size_t ret_val_size;
    clGetProgramBuildInfo(prog, device, CL_PROGRAM_BUILD_LOG, 0, 0, &ret_val_size);
    build_log = malloc((ret_val_size + 1)*sizeof(char));
    clGetProgramBuildInfo(prog, device, CL_PROGRAM_BUILD_LOG, ret_val_size, build_log, 0);
    printf("%s\n", build_log);
    free(build_log);
  }
  free(src);
  return prog;
}

void print_opencl_error(cl_int error){
  if (error){
    printf("OpenCL %d error!\n", error);
    fflush(stdout);
  }
}

FILE* fopen_and_check(char* filename, char* mode){
  FILE* file = fopen(filename, mode);
  if (!file){
    printf("Error: can't open %s file!\n", filename);
    exit(0);
  }
  return file;
}
