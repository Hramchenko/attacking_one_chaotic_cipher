#include "hvalues_writer.h"

#include <stdio.h>
#include <map>

extern "C"{
#include "hopencl_functions.h"
#include "hglobal_constants.h"
}

using namespace std;

hvalues_writer_t* hvalues_writer_create(char* filename){
  hvalues_writer_t* result = (hvalues_writer_t*)malloc(sizeof(hvalues_writer_t));
  result->values_dict = new map<float, char>;
  result->values_processed = 0;
  hvalues_writer_start_writing(result, filename);
  result->values_written = 0;
  return result;
}

void hvalues_writer_start_writing(hvalues_writer_t* writer, char* filename){
  writer->output = fopen_and_check(filename, (char*)"wb");
//  writer->values_written = 0;
}

void hvalues_writer_filter_data(hvalues_writer_t* writer, float* data, size_t values_count){
  size_t index;
  float value;
  map<float, char>* values_dict = (map<float, char>*)writer->values_dict;
  for (index = 0; index < values_count; index++){
    value = data[index];
    if (value > -128){
      (*values_dict)[value] = 1;
    }
    writer->values_processed += 1;
  }
}

void hvalues_writer_write_filtered_data(hvalues_writer_t* writer){
  if (writer->values_processed >= MAX_OUT_DICT_SIZE){
    map<float, char>* values_dict = (map<float, char>*)writer->values_dict;
    map<float, char>::iterator map_iter;
    float value;
    for (map_iter = values_dict->begin(); map_iter != values_dict->end(); map_iter++){
      value = map_iter->first;
      writer->values_written += 1;
      fwrite(&value, sizeof(float), 1, writer->output);
    }
    writer->values_processed = 0;
    values_dict->clear();
  }
}

void hvalues_writer_write_data(hvalues_writer_t* writer, float* data, size_t values_count){
  hvalues_writer_filter_data(writer, data, values_count);
  hvalues_writer_write_filtered_data(writer);
}

void hvalues_writer_finish_writing(hvalues_writer_t* writer){
  if(!writer->output)
    return;
  writer->values_processed = MAX_OUT_DICT_SIZE + 1;
  hvalues_writer_write_filtered_data(writer);
  fclose(writer->output);
  writer->output = 0;
}

void hvalues_writer_free(hvalues_writer_t* writer){
  hvalues_writer_finish_writing(writer);
  printf("Written %u\n", writer->values_written);
  delete (map<float, char>*)writer->values_dict;
  free(writer);
}

