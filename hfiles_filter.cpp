#include "hfiles_filter.h"

#include <map>

extern "C"{

#include "hvalues_reader.h"
#include "hvalues_writer.h"
#include "hglobal_constants.h"

}

using namespace std;

void hfiles_filter_process_data(float* values, size_t* values_count){
  unsigned int i;
  map<float, char> values_dict;
  float value;
  for (i = 0; i < (*values_count); i++){
    value = values[i];
    values_dict[value] = 1;
  }
  *values_count = values_dict.size();
  map<float, char>::iterator map_iter;

  for (map_iter = values_dict.begin(), i = 0; map_iter != values_dict.end(); map_iter++, i++){
    value = map_iter->first;
    values[i] = value;
  }
}

extern "C" void hfiles_filter_exec(char* input, char* output){
  map<float, char> values_dict;
  hvalues_reader_t* reader = hvalues_reader_create(input);
  hvalues_writer_t* writer = hvalues_writer_create(output);
  size_t block_size = MAX_FILTER_BLOCK_SIZE;
  float* reading_buffer = (float*)malloc(block_size*sizeof(float));
  size_t bytes_read = hvalues_reader_read_data(reader, reading_buffer, block_size);
  size_t values_processed = 0;
  size_t new_count = 0;
  size_t count;
  while (bytes_read){
    count = bytes_read;
    hfiles_filter_process_data(reading_buffer, &count);
    hvalues_writer_write_data(writer, reading_buffer, count);
    new_count += count;

    values_processed += bytes_read;
    printf("Values processed %u. Appended: %u.\n", values_processed, count);

    bytes_read = hvalues_reader_read_data(reader, reading_buffer, block_size);
  }
  printf("Values appended: %u.\n", new_count);
  hvalues_reader_free(reader);
  hvalues_writer_free(writer);
}
