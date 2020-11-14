#include "hlogistic_generator.h"


#include <stdlib.h>
#include <stdio.h>

#include "hbits.h"
#include "hglobal_constants.h"

void hlogistic_generator_init_data(hlogistic_generator_t* logistic);

void hlogistic_generator_out(FILE* out, float start, unsigned char* key, unsigned char* generators, unsigned int key_length, float* x_values, unsigned char* gamma_values, unsigned int gamma_size){
  fprintf(out, "Gamma %x.\n",  *((unsigned int*)&start));
  fprintf(out, "X(FLOAT)     X(UIUNT)     KEY[i]     GEN[i]     GAMMA(UCHAR)\n");
  int i;
  for (i = 0; i < gamma_size; i++){
    unsigned int gamma = *((unsigned int*)&x_values[i]);
    fprintf(out, "%.6e      %0x     %.2x     %.1d     %.2x\n", x_values[i], gamma, (unsigned int)key[i%key_length], (unsigned int)generators[i%key_length], (unsigned int)gamma_values[i]);
  }
}

void hlogistic_generator_exec(char* input, char* output){
  FILE* in = fopen_and_check(input, "r");
  float start;
  unsigned int key_length;
  unsigned int gamma_size;
  fscanf(in, "%f", &start);
  fscanf(in, "%u", &key_length);
  fscanf(in, "%u", &gamma_size);
  unsigned char* key = malloc(key_length*sizeof(unsigned char));
  unsigned char* generators = malloc(key_length*sizeof(unsigned char));
  int i;
  unsigned int key_symbol;
  unsigned int generator_symbol;
  for (i = 0; i < key_length; i++){
    fscanf(in, "%x", &key_symbol);
    key[i] = key_symbol;
    fscanf(in, "%d", &generator_symbol);
    generators[i] = generator_symbol;
    if (generator_symbol > 3){
      printf("Error: wrong generator type %d!\n", (unsigned int)generator_symbol);
      exit(0);
    }
  }
  fclose(in);

  float* x_values = malloc(gamma_size*sizeof(float));
  unsigned char* gamma_values = malloc(gamma_size*sizeof(unsigned char));
  hlogistic_generator_t* generator = hlogistic_generator_create(start, key, generators, key_length, x_values, gamma_values, gamma_size);
  hlogistic_generator_init(generator);
  hlogistic_generator_process(generator);
  hlogistic_generator_print_result(generator);
  hlogistic_generator_free(generator);
  FILE* out = fopen_and_check(output, "w");
  hlogistic_generator_out(out, start, key, generators, key_length, x_values, gamma_values, gamma_size);
  fclose(out);
  free(x_values);
  free(gamma_values);
  free(key);
  free(generators);
}

hlogistic_generator_t* hlogistic_generator_create(float x_0, unsigned char* key, unsigned char* generators, unsigned int key_length, float* x_values, unsigned char* gamma_values, int values_count){
  hlogistic_generator_t* result = malloc(sizeof(hlogistic_generator_t));
  result->opencl_program = hopencl_program_create();
  result->x_0 = x_0;
  result->key = key;
  result->generators = generators;
  result->key_length = key_length;
  result->values_count = values_count;
  result->x_values = x_values;
  result->gamma_values = gamma_values;
  return result;
}

void hlogistic_generator_init(hlogistic_generator_t* logistic){
  hopencl_program_init_device(logistic->opencl_program, 0, 0);
  hopencl_program_load(logistic->opencl_program, "./cl/hlogistic_generator.cl.c", "-w -I./cl", "hlogistic_generator");
  hlogistic_generator_init_data(logistic);
}

void hlogistic_generator_init_data(hlogistic_generator_t* logistic){
  size_t count = logistic->values_count;
  hopencl_set_kernel_argument(logistic->opencl_program, &logistic->x_0, sizeof(float), 0);
  hopencl_program_create_buffer(logistic->opencl_program, &logistic->mem_key_input, logistic->key_length*sizeof(unsigned char), CL_MEM_READ_ONLY, 1);
  hopencl_program_write_to_buffer(logistic->opencl_program, logistic->mem_key_input, logistic->key, logistic->key_length*sizeof(unsigned char));
  hopencl_program_create_buffer(logistic->opencl_program, &logistic->mem_generators_input, logistic->key_length*sizeof(unsigned char), CL_MEM_READ_ONLY, 2);
  hopencl_program_write_to_buffer(logistic->opencl_program, logistic->mem_generators_input, logistic->generators, logistic->key_length*sizeof(unsigned char));
  hopencl_set_kernel_argument(logistic->opencl_program, &logistic->key_length, sizeof(unsigned int), 3);
  hopencl_set_kernel_argument(logistic->opencl_program, &logistic->values_count, sizeof(unsigned int), 4);
  hopencl_program_create_buffer(logistic->opencl_program, &logistic->mem_x_output, count*sizeof(float), CL_MEM_WRITE_ONLY, 5);
  hopencl_program_create_buffer(logistic->opencl_program, &logistic->mem_gamma_output, count*sizeof(unsigned char), CL_MEM_WRITE_ONLY, 6);
}

void hlogistic_generator_process(hlogistic_generator_t* logistic){
  size_t data_dimension = 128;
  hopencl_program_start_kernel(logistic->opencl_program, 1, &data_dimension, 0);
  hopencl_program_read_from_buffer(logistic->opencl_program, logistic->mem_x_output, logistic->x_values, logistic->values_count*sizeof(float));
  hopencl_program_read_from_buffer(logistic->opencl_program, logistic->mem_gamma_output, logistic->gamma_values, logistic->values_count*sizeof(unsigned char));
  hopencl_program_finish(logistic->opencl_program);
}

void hlogistic_generator_print_result(hlogistic_generator_t* logistic){
  int i;
  printf("Generator x_0 = %f:\n", logistic->x_0);
  for(i = 0; i < logistic->values_count; i++){
    printf("%e   %d    ", logistic->x_values[i], (unsigned int)logistic->gamma_values[i]);
    print_float_bits(logistic->x_values[i]);
    printf("  ");
    print_char_bits(logistic->gamma_values[i]);
    printf("\n");
  }
}

void hlogistic_generator_free(hlogistic_generator_t* logistic){
  hopencl_program_free_buffer(logistic->mem_key_input);
  hopencl_program_free_buffer(logistic->mem_generators_input);
  hopencl_program_free_buffer(logistic->mem_x_output);
  hopencl_program_free_buffer(logistic->mem_gamma_output);
  hopencl_program_free(logistic->opencl_program);
  free(logistic);
}

