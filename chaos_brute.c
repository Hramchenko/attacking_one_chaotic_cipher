#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <getopt.h>

#include "hinitial_ic_generator.h"
#include "hlogistic_generator.h"
#include "hgamma_analyzer.h"
#include "hkey_checker.h"
#include "hfiles_filter.h"
#include "hdata_viewer.h"
#include "hcipher_all.h"
#include "hvalid_keys.h"

void print_usage(char* program_name){
  printf("Usage %s [OPTIONS]:\n", program_name);
  printf("-g  --gen-gamma  <input> <output>                          Generate gamma with start from file.\n");
  printf("-f  --filter-file  <input> <output>                        Remove doubles from file.\n");
  printf("-t  --gen-initial  <output>                                Generate initial layer.\n");
  printf("-p  --process-layer <next_gamma> <input> <output>          Process layer.\n");
  printf("-i  --opencl-info                                          Print OpenCL info.\n");
  printf("-e  --check-key <key> <input>                              Check key.\n");
  printf("-l  --cipher-all <start_x> <output>                        Cipher start_x with all keys and all maps.\n");
  printf("-y  --valid-keys <next_gamma> <input> <output>             Write all valid keys in file.\n");
  printf("-v  --view-file <input>                                    Print file content.\n");
  printf("-n  --map-number   <number>                                Select chaotic map type:\n");
  printf("                                                               0: logistic,\n");
  printf("                                                               1: tent,\n");
  printf("                                                               0: sine,\n");
  printf("                                                               0: cubic.\n");
  printf("-h  --help                                                 Print this information.\n");
}

void check_args_count(int argc, char** argv, int count){
  if (argc - 2 < count){
    printf("Error: wrong arguments count!\n");
    print_usage(argv[0]);
    exit(0);
  }
}

int main(int argc, char** argv){
  if (argc == 1){
    print_usage(argv[0]);
    return 0;
  }
  const char* short_options = "hygtpieflvn:";
  const struct option long_options[] =
  {
   {"help", 0, 0, 'h'},
   {"gen-gamma", 0, 0, 'g'},
   {"gen-initial", 0, 0, 't'},
   {"process-layer", 0, 0, 'p'},
   {"opencl-info", 0, 0, 'i'},
   {"check-key", 0, 0, 'e'},
   {"filter-file", 0, 0, 'f'},
   {"view-file", 0, 0, 'v'},
   {"map-number", 1, 0, 'm'},
   {"cipher-all", 0, 0, 'l'},
   {"valid-keys", 0, 0, 'y'},
   {0, 0, 0, 0}
  };
  unsigned int map_number = 0;
  int next_option = getopt_long(argc, argv, short_options, long_options, 0);
  while (next_option){
    if ((next_option == 'h') || (next_option == '?')){
      print_usage(argv[0]);
      return 0;
    }
    else if (next_option == 'n'){
      map_number = atoi(optarg);
      if (map_number > 3){
        printf("Invalid map number.\n");
        print_usage(argv[0]);
        return 0;
      }
    }
    else if (next_option == 'i'){
      print_opencl_info();
      return 0;
    }
    else if (next_option == 'e'){
      check_args_count(argc, argv, 2);
      hkey_checker_exec(argv[argc - 2], argv[argc - 1]);
      return 0;
    }
    else if (next_option == 'v'){
      check_args_count(argc, argv, 1);
      hdata_viewer_exec(argv[argc - 1]);
      return 0;
    }
    if (next_option == 'g'){
      check_args_count(argc, argv, 2);
      hlogistic_generator_exec(argv[argc - 2], argv[argc - 1]);
      return 0;
    }
    if (next_option == 'l'){
      check_args_count(argc, argv, 2);
      hcipher_all_exec(argv[argc - 2], argv[argc - 1]);
      return 0;
    }
    else if (next_option == 'f'){
      hfiles_filter_exec(argv[argc - 2], argv[argc - 1]);
      return 0;
    }
    else if(next_option == 't'){
      check_args_count(argc, argv, 1);
      hinitial_ic_generator_exec(argv[argc - 1]);
      return 0;
    }
    else if(next_option == 'p'){
      int next_gamma = strtol(argv[argc - 3], 0, 16);
      hgamma_analyzer_exec(argv[argc - 2], argv[argc - 1], next_gamma, map_number);
      return 0;
    }
    else if(next_option == 'y'){
      int next_gamma = strtol(argv[argc - 3], 0, 16);
      hvalid_keys_exec(argv[argc - 2], argv[argc - 1], next_gamma, map_number);
      return 0;
    }
    next_option = getopt_long(argc, argv, short_options, long_options, 0);
  }
  return 0;
}

