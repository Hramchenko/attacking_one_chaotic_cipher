#include "hdata_viewer.h"

#include <stdio.h>
#include <stdlib.h>

#include "hvalues_reader.h"

void hdata_viewer_exec(char* input){
  hvalues_reader_t* layer = hvalues_reader_create(input);
  size_t block_size = 1024;
  float* reading_buffer = malloc(block_size*sizeof(float));
  size_t bytes_read = hvalues_reader_read_data(layer, reading_buffer, block_size);
  size_t count = 0;
  int index;
  int int_val;
  float float_val;
  while (bytes_read){
    for (index = 0; index < bytes_read; index++){
      float_val = reading_buffer[index];
      int_val = *((unsigned int*)&float_val);
      printf("%0x   %e \n", int_val, float_val);
    }
    count += bytes_read;
    printf("Values count %u.\n", count);
    bytes_read = hvalues_reader_read_data(layer, reading_buffer, block_size);
  }
  hvalues_reader_free(layer);
  free(reading_buffer);
}
