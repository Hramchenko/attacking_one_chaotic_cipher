#include "hbuild_options.h"

void hopencl_program_load_for_map(hopencl_program_t* program, char* file_name, char* kernel_name, unsigned char map_number){
  char* options;
  if (map_number == 0){
    options = "-w -I./cl -D MAP_TYPE=0";
  }
  else if (map_number == 1){
    options = "-w -I./cl -D MAP_TYPE=1";
  }
  else if (map_number == 2){
    options = "-w -I./cl -D MAP_TYPE=2";
  }
  else{
    options = "-w -I./cl -D MAP_TYPE=3";
  }
  hopencl_program_load(program, file_name, options, kernel_name);
}
