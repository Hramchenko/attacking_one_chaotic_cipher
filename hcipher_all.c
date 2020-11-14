#include "hcipher_all.h"

#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "hbuild_options.h"
#include "hglobal_constants.h"

void hcipher_all_print(hcipher_all_t* cipher, FILE* out, float start_x, unsigned char map_number){
  int i;
  unsigned int start = *((unsigned int*)&start_x);
  for (i = 0; i < 256; i++){
    fprintf(out, "%x      %.2x      %.1u          %x      %.2x \n", start, i, map_number, *((unsigned int*)&cipher->result_x[i]), (unsigned int)cipher->result_gamma[i]);
  }
}

void hcipher_all_exec(char* start, char* output){
  unsigned int int_start = strtol(start, 0, 16);
  float start_x = *((float*)&int_start);
  FILE* out_file = fopen_and_check(output, "w");
  fprintf(out_file, "start_x      key      map      result      gamma \n");
  unsigned char map_number;
  hcipher_all_t* cipher;
  for (map_number = 0; map_number <= 3; map_number++){
    cipher = hcipher_all_create(map_number);
    hcipher_all_init(cipher);
    hcipher_all_process(cipher, start_x);
    hcipher_all_print(cipher, out_file, start_x, map_number);
    hcipher_all_free(cipher);
  }
  fclose(out_file);
}

hcipher_all_t* hcipher_all_create(unsigned char map_number){
  hcipher_all_t* result = malloc(sizeof(hcipher_all_t));
  result->opencl_program = hopencl_program_create();
  result->result_x = malloc(256*sizeof(float));
  result->result_gamma = malloc(256*sizeof(unsigned char));
  result->map_number = map_number;
  return result;
}

void hcipher_all_init_data(hcipher_all_t* cipher){
  hopencl_program_create_buffer(cipher->opencl_program, &cipher->x_output, 256*sizeof(float), CL_MEM_WRITE_ONLY, 1);
  hopencl_program_create_buffer(cipher->opencl_program, &cipher->gamma_output, 256*sizeof(unsigned char), CL_MEM_WRITE_ONLY, 2);
}

void hcipher_all_init(hcipher_all_t* cipher){
  hopencl_program_init_device(cipher->opencl_program, 0, 0);
  hopencl_program_load_for_map(cipher->opencl_program, "./cl/hcipher_all.cl.c", "cipher_all", cipher->map_number);
  hcipher_all_init_data(cipher);
}

void hcipher_all_process(hcipher_all_t* cipher, float start_x){
  hopencl_set_kernel_argument(cipher->opencl_program, &start_x, sizeof(float), 0);
  size_t data_dimension = 1;
  hopencl_program_start_kernel(cipher->opencl_program, 1, &data_dimension, 0);
  hopencl_program_read_from_buffer(cipher->opencl_program, cipher->x_output, cipher->result_x, 256*sizeof(float));
  hopencl_program_read_from_buffer(cipher->opencl_program, cipher->gamma_output, cipher->result_gamma, 256*sizeof(unsigned char));
  hopencl_program_finish(cipher->opencl_program);
}

void hcipher_all_free(hcipher_all_t* cipher){
  free(cipher->result_x);
  free(cipher->result_gamma);
  hopencl_program_free_buffer(cipher->x_output);
  hopencl_program_free_buffer(cipher->gamma_output);
  hopencl_program_free(cipher->opencl_program);
  free(cipher);
}
