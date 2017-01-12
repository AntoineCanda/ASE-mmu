#ifndef _SWAP_H_
#define _SWAP_H_
#include <hardware.h>
#include "hw_config.h"
#include <stdio.h>
#include <string.h>


 static FILE *swap_file;

 char init_swap(const char *path);
 char store_to_swap( int vpage,  int ppage);
 char fetch_from_swap( int vpage,  int ppage);

 #endif
