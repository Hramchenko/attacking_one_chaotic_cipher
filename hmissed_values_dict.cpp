#include "hmissed_values_dict.h"

#include <stdio.h>
#include <map>

extern "C"{
#include "hopencl_functions.h"
#include "hglobal_constants.h"
}

using namespace std;

hmissed_values_dict_t* hmissed_values_dict_create(hgamma_analyzer_t* gamma_analyzer, hvalues_writer_t* values_writer, unsigned char next_gamma, size_t out_block_size){
  hmissed_values_dict_t* result = (hmissed_values_dict_t*)malloc(sizeof(hmissed_values_dict_t));
  result->gamma_analyzer = gamma_analyzer;
  result->values_writer = values_writer;
  result->out_block_size = out_block_size;
  result->block_multiplier = 1;
  result->values_dict = new map<float, char>;
  result->next_dict = new map<float, char>;
  result->input_buffer = (float*)malloc(MAX_ANALYZER_BLOCK*sizeof(float));
  result->next_gamma = next_gamma;
  return result;
}

void hmissed_values_dict_append_values(float* data, size_t values_count, void* output_dict){
  size_t index;
  size_t good=0;
  size_t bad = 0;
  map<float, char>* values_dict = (map<float, char>*)output_dict;
  for (index = 0; index < values_count; index++){
    if (data[index] == -512){
//     printf("\n%x    %f    %u\n", *((unsigned int*)&data[index + 1]), data[index + 1], index);
     (*values_dict)[data[index + 1]] = 1;
      bad++;
    }
    else
      good++;
  }
  printf("Good %u bad %u\n", good, bad);
}

void hmissed_values_dict_process_block(hmissed_values_dict_t* missed_dict, size_t buffer_index, size_t out_block_size){
//    unsigned int i;
//    for (i = 0; i < buffer_index; i++){
//      printf("fltr %x    \n", *((unsigned int*)& missed_dict->input_buffer[i]));
//
//    }
  hgamma_analyzer_process(missed_dict->gamma_analyzer, missed_dict->input_buffer, buffer_index + 1, missed_dict->next_gamma, out_block_size);
  hmissed_values_dict_append_values(missed_dict->gamma_analyzer->result_values, (buffer_index + 1)*out_block_size, missed_dict->next_dict);
  hvalues_writer_write_data(missed_dict->values_writer, missed_dict->gamma_analyzer->result_values, (buffer_index + 1)*out_block_size);
}

void hmissed_values_dict_filter(hmissed_values_dict_t* missed_dict){
  map<float, char>* values_dict = (map<float, char>*)missed_dict->values_dict;
  map<float, char>::iterator map_iter;
  size_t buffer_index = 0;
  size_t out_block_size = missed_dict->out_block_size*missed_dict->block_multiplier;
  size_t max_input_block = MAX_ANALYZER_BLOCK/out_block_size;
  for (map_iter = values_dict->begin(); map_iter != values_dict->end(); map_iter++){
    missed_dict->input_buffer[buffer_index] = map_iter->first;
    if (buffer_index >= max_input_block){
      hmissed_values_dict_process_block(missed_dict, buffer_index, out_block_size);
      buffer_index = 0;
    }
    else
      buffer_index++;
  }
  if (buffer_index >= 1){
    hmissed_values_dict_process_block(missed_dict, buffer_index - 1, out_block_size);
  }
  values_dict->clear();
  void* tmp;
  tmp = missed_dict->values_dict;
  missed_dict->values_dict = missed_dict->next_dict;
  missed_dict->next_dict = tmp;
}

size_t hmissed_values_dict_size(hmissed_values_dict_t* missed_dict){
  map<float, char>* values_dict = (map<float, char>*)missed_dict->values_dict;
  size_t result = values_dict->size();
  return result;
}

void hmissed_values_dict_process_data(hmissed_values_dict_t* missed_dict, float* data, size_t values_count){
  hmissed_values_dict_append_values(data, values_count, missed_dict->values_dict);
  missed_dict->block_multiplier = 1;
  while(hmissed_values_dict_size(missed_dict)){
    missed_dict->block_multiplier *= 4;
    hmissed_values_dict_filter(missed_dict);
  }
}

void hmissed_values_dict_finish_processing(hmissed_values_dict_t* values_dict){
}

void hmissed_values_dict_free(hmissed_values_dict_t* values_dict){
  hmissed_values_dict_finish_processing(values_dict);
  delete (map<float, char>*)values_dict->values_dict;
  delete (map<float, char>*)values_dict->next_dict;
  free(values_dict->input_buffer);
  free(values_dict);
}
