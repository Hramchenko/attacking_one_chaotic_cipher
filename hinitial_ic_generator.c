#include "hinitial_ic_generator.h"

#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "hbuild_options.h"
#include "hglobal_constants.h"
#include "hvalues_writer.h"

void hinitial_ic_generator_exec(char* output){
  hkey_ic_generator_t* key_generator = hkey_ic_generator_create();
  hkey_ic_generator_generate(key_generator);
  hvalues_writer_t* writer = hvalues_writer_create(output);
  hvalues_writer_write_data(writer, key_generator->keys_block, key_generator->keys_count);
  hvalues_writer_free(writer);
}

