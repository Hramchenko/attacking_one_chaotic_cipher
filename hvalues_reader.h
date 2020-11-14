#ifndef __HVALUES_READER_H__
#define __HVALUES_READER_H__

#include <stdio.h>
#include <stdlib.h>

struct _hvalues_reader{
  FILE* input;
};

typedef struct _hvalues_reader hvalues_reader_t;

hvalues_reader_t* hvalues_reader_create(char* filename);

void hvalues_reader_start_reading(hvalues_reader_t* reader, char* filename);
size_t hvalues_reader_read_data(hvalues_reader_t* reader, float* destination, size_t block_size);
void hvalues_reader_finish_reading(hvalues_reader_t* reader);

extern void hvalues_reader_free(hvalues_reader_t* reader);


#endif
