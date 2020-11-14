#include <stdio.h>
#include "hbits.h"

template<class T> static void print_bits(T var){
  unsigned int b;
  unsigned int* int_var_ptr = (unsigned int*)&var;
  unsigned int int_var = *(int_var_ptr);
  unsigned int bits_count = sizeof(T)*8;
  unsigned int mask = 1 << (bits_count - 1);
  for (b = 1; b <= bits_count; ++b){
    printf("%c", int_var & mask ? '1' : '0');
    int_var <<= 1;
//    if (b % 8 == 0)
//      putchar(' ');
  }
}

extern "C" void print_int_bits(unsigned int var){
  print_bits<unsigned int>(var);
}

extern "C" void print_float_bits(float var){
  print_bits<float>(var);
}

extern "C" void print_char_bits(unsigned char var){
  print_bits<unsigned char>(var);
}
