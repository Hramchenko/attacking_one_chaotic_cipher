#ifndef __HVALUES_WRITER_H__
#define __HVALUES_WRITER_H__

#include <stdio.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif


struct _hvalues_writer{
  size_t values_written;
  void* values_dict;
  size_t values_processed;
  FILE* output;
};

typedef struct _hvalues_writer hvalues_writer_t;

extern hvalues_writer_t* hvalues_writer_create(char* filename);

void hvalues_writer_write_data(hvalues_writer_t* writer, float* data, size_t values_count);
void hvalues_writer_start_writing(hvalues_writer_t* writer, char* filename);
void hvalues_writer_finish_writing(hvalues_writer_t* writer);
void hvalues_writer_free(hvalues_writer_t* writer);

#ifdef __cplusplus
}
#endif

#endif
