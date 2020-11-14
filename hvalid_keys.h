#ifndef __HVALID_KEYS_H__
#define __HVALID_KEYS_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "hcipher_all.h"

struct _hvalid_keys{
  void* keys_dict;
  hcipher_all_t* cipher;
  unsigned char next_gamma;
};

typedef struct _hvalid_keys hvalid_keys_t;

extern hvalid_keys_t* hvalid_keys_create(unsigned char next_gamma, unsigned char map_number);
extern void hvalid_keys_process(hvalid_keys_t* cipher, float start_x);
extern void hvalid_keys_free(hvalid_keys_t* cipher);
extern void hvalid_keys_init(hvalid_keys_t* cipher);
extern void hvalid_keys_exec(char* input, char* output, unsigned char next_gamma, unsigned char map_number);

#ifdef __cplusplus
}
#endif

#endif
