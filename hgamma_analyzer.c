#include "hgamma_analyzer.h"

#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "hbuild_options.h"
#include "hmissed_values_dict.h"
#include "hglobal_constants.h"
#include "hvalues_reader.h"
#include "hvalues_writer.h"

void hgamma_analyzer_exec(char* input, char* output, unsigned char next_gamma, unsigned char map_number){
  hvalues_reader_t* reader = hvalues_reader_create(input);
  hvalues_writer_t* writer = hvalues_writer_create(output);
  size_t default_size = 4;
  size_t block_size = MAX_ANALYZER_BLOCK/default_size;
  hgamma_analyzer_t* analyzer = hgamma_analyzer_create(MAX_ANALYZER_BLOCK, map_number);
  hmissed_values_dict_t* missed_dict = hmissed_values_dict_create(analyzer, writer, next_gamma, default_size);
  hgamma_analyzer_init(analyzer);
  float* reading_buffer = malloc(block_size*sizeof(float));
  size_t bytes_read = hvalues_reader_read_data(reader, reading_buffer, block_size);
  size_t bytes_processed = 0;
  while (bytes_read){
    printf("Values processed: %u. Written: %u. Filtered %f.\n", bytes_processed, writer->values_written, 1 - ((float)writer->values_written)/bytes_processed);
    hgamma_analyzer_process(analyzer, reading_buffer, bytes_read, next_gamma, default_size);
    hvalues_writer_write_data(writer, analyzer->result_values, bytes_read*default_size);
    hmissed_values_dict_process_data(missed_dict, analyzer->result_values, bytes_read*default_size);
    bytes_read = hvalues_reader_read_data(reader, reading_buffer, block_size);
    bytes_processed += bytes_read;
  }
  hvalues_reader_free(reader);
  hvalues_writer_free(writer);
  hgamma_analyzer_free(analyzer);
  free(reading_buffer);
}

hgamma_analyzer_t* hgamma_analyzer_create(size_t result_block_size, unsigned char map_number){
  hgamma_analyzer_t* result = malloc(sizeof(hgamma_analyzer_t));
  result->opencl_program = hopencl_program_create();
  result->result_values = malloc(result_block_size*sizeof(float));
  result->result_block_size = result_block_size;
  result->map_number = map_number;
  return result;
}

void hgamma_analyzer_init_data(hgamma_analyzer_t* analyzer){
  size_t buffers_size = analyzer->result_block_size*sizeof(float);
  hopencl_program_create_buffer(analyzer->opencl_program, &analyzer->mem_input, buffers_size, CL_MEM_READ_ONLY, 0);
  hopencl_program_create_buffer(analyzer->opencl_program, &analyzer->mem_output, buffers_size, CL_MEM_WRITE_ONLY, 3);
}

void hgamma_analyzer_init(hgamma_analyzer_t* analyzer){
  hopencl_program_init_device(analyzer->opencl_program, 0, 0);
  hopencl_program_load_for_map(analyzer->opencl_program, "./cl/hgamma_analyzer.cl.c", "gamma_analyzer", analyzer->map_number);
  hgamma_analyzer_init_data(analyzer);
}

void hgamma_analyzer_process(hgamma_analyzer_t* analyzer, float* initial_values, size_t initial_count, unsigned char next_gamma, unsigned int out_block_size){
  analyzer->out_block_size = out_block_size;
  unsigned char max_out_block_index = out_block_size - 1;
  hopencl_set_kernel_argument(analyzer->opencl_program, &next_gamma, sizeof(unsigned char), 1);
  hopencl_set_kernel_argument(analyzer->opencl_program, &max_out_block_index, sizeof(unsigned char), 2);
  hopencl_program_write_to_buffer(analyzer->opencl_program, analyzer->mem_input, initial_values, initial_count*sizeof(float));
  size_t data_dimension = initial_count;
  hopencl_program_start_kernel(analyzer->opencl_program, 1, &data_dimension, 0);
  hopencl_program_read_from_buffer(analyzer->opencl_program, analyzer->mem_output, analyzer->result_values, out_block_size*initial_count*sizeof(float));
  hopencl_program_finish(analyzer->opencl_program);
}

void hgamma_analyzer_free(hgamma_analyzer_t* analyzer){
  free(analyzer->result_values);
  hopencl_program_free_buffer(analyzer->mem_input);
  hopencl_program_free_buffer(analyzer->mem_output);
  hopencl_program_free(analyzer->opencl_program);
  free(analyzer);
}
