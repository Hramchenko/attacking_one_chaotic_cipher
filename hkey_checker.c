#include "hkey_checker.h"

#include <stdio.h>
#include <stdlib.h>

#include "hvalues_reader.h"
#include "hglobal_constants.h"

void hkey_checker_exec(char* string_key, char* input){
  hvalues_reader_t* reader = hvalues_reader_create(input);
  unsigned int key = strtol(string_key, 0, 16);
  float float_key = *((float*)&key);
  size_t block_size = MAX_KEY_CHECKER_BLOCK;
  float* reading_buffer = malloc(block_size*sizeof(float));
  size_t bytes_read = hvalues_reader_read_data(reader, reading_buffer, block_size);
  size_t values_processed = 0;
  int index;
  printf("%x\n", key);
  while (bytes_read){
    for (index = 0; index < bytes_read; index++){
      if (reading_buffer[index] == float_key){
        printf("Key %x was founded at %u position.\n", key, index);
        return;
      }
    }
    values_processed += bytes_read;
    printf("Values processed %u.\n", values_processed);

    bytes_read = hvalues_reader_read_data(reader, reading_buffer, block_size);
  }
  printf("Key %x was not founded!\n", key);
  hvalues_reader_free(reader);
  free(reading_buffer);
}
