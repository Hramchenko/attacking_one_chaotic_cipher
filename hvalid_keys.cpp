#include "hvalid_keys.h"

extern "C"{
#include  "hcipher_all.h"
#include "hvalues_reader.h"
#include "hglobal_constants.h"
}

#include <map>

using namespace std;

void hvalid_keys_print(hvalid_keys_t* valid_keys, FILE* out){
  map<unsigned char, char>* keys_dict = (map<unsigned char, char>*)valid_keys->keys_dict;
  map<unsigned char, char>::iterator map_iter;
  printf("Keys written: %d.\n",keys_dict->size());
  for (map_iter = keys_dict->begin(); map_iter != keys_dict->end(); map_iter++){
    fprintf(out, "%.2x\n", map_iter->first);
  }
}

void hvalid_keys_exec(char* input, char* output, unsigned char next_gamma, unsigned char map_number){
  hvalues_reader_t* reader = hvalues_reader_create(input);
  size_t block_size = MAX_FILTER_BLOCK_SIZE;
  float* reading_buffer = (float*)malloc(block_size*sizeof(float));
  FILE* out_file = fopen_and_check(output, (char*)"w");
  hvalid_keys_t* valid_keys = hvalid_keys_create(next_gamma, map_number);
  hvalid_keys_init(valid_keys);

  size_t bytes_read = hvalues_reader_read_data(reader, reading_buffer, block_size);
  unsigned int index;
  while (bytes_read){
    for (index = 0; index < bytes_read; index++){
      hvalid_keys_process(valid_keys, reading_buffer[index]);
    }
    bytes_read = hvalues_reader_read_data(reader, reading_buffer, block_size);
  }
  hvalid_keys_print(valid_keys, out_file);
  hvalid_keys_free(valid_keys);
  hvalues_reader_free(reader);
  fclose(out_file);
  free(reading_buffer);
}

hvalid_keys_t* hvalid_keys_create(unsigned char next_gamma, unsigned char map_number){
  hvalid_keys_t* result = (hvalid_keys_t*)malloc(sizeof(hvalid_keys_t));
  result->cipher = hcipher_all_create(map_number);
  result->keys_dict = new map<unsigned char, char>;
  result->next_gamma = next_gamma;
  return result;
}

void hvalid_keys_init(hvalid_keys_t* valid_keys){
  hcipher_all_init(valid_keys->cipher);
}


void hvalid_keys_process(hvalid_keys_t* valid_keys, float start_x){
  hcipher_all_process(valid_keys->cipher, start_x);
  unsigned int i;
  map<unsigned char, char>* keys_dict = (map<unsigned char, char>*)valid_keys->keys_dict;
  for (i = 0; i < 256; i++){
    if (valid_keys->cipher->result_gamma[i] == valid_keys->next_gamma)
      (*keys_dict)[i] = 1;
  }
}

void hvalid_keys_free(hvalid_keys_t* valid_keys){
  hcipher_all_free(valid_keys->cipher);
  delete (map<unsigned char, char>*)valid_keys->keys_dict;
  free(valid_keys);
}
