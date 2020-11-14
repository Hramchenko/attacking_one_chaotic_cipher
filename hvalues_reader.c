#include "hvalues_reader.h"

#include <stdio.h>

#include "hopencl_functions.h"

hvalues_reader_t* hvalues_reader_create(char* filename){
  hvalues_reader_t* result = (hvalues_reader_t*)malloc(sizeof(hvalues_reader_t));
  hvalues_reader_start_reading(result, filename);
  return result;
}

void hvalues_reader_start_reading(hvalues_reader_t* reader, char* filename){
  reader->input = fopen_and_check(filename, (char*)"rb");
}

void hvalues_reader_finish_reading(hvalues_reader_t* reader){
  if (!reader->input)
    return;
  fclose(reader->input);
  reader->input = 0;
}

size_t hvalues_reader_read_data(hvalues_reader_t* reader, float* destination, size_t block_size){
  size_t values_read;
  size_t count;
  for(values_read = 0; values_read < block_size; values_read++){
    count = fread(&destination[values_read], sizeof(float), 1, reader->input);
    if (!count)
      return values_read;
  }
  return values_read;
}

void hvalues_reader_free(hvalues_reader_t* reader){
  hvalues_reader_finish_reading(reader);
  free(reader);
}
