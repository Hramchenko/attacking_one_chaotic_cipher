#ifndef __HMISSED_VALUES_DICT_H__
#define __HMISSED_VALUES_DICT_H__

#include <stdio.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

#include "hgamma_analyzer.h"
#include "hvalues_writer.h"

struct _hmissed_values_dict{
  void* values_dict;
  void* next_dict;
  float* input_buffer;
  hgamma_analyzer_t* gamma_analyzer;
  hvalues_writer_t* values_writer;
  size_t out_block_size;
  size_t block_multiplier;
  size_t max_input_block;
  unsigned char next_gamma;
};

typedef struct _hmissed_values_dict hmissed_values_dict_t;

extern hmissed_values_dict_t* hmissed_values_dict_create(hgamma_analyzer_t* gamma_analyzer, hvalues_writer_t* values_writer, unsigned char next_gamma, size_t out_block_size);
void hmissed_values_dict_process_data(hmissed_values_dict_t* missed_dict, float* data, size_t values_count);
void hmissed_values_dict_free(hmissed_values_dict_t* values_dict);

#ifdef __cplusplus
}
#endif


#endif
