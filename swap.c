#include "swap.h"


 char init_swap(const char *path){
  swap_file = fopen(path, "w+");
  if(swap_file == NULL){
	  return -1;
  }

  return 0;
}

 char store_to_swap( int vpage,  int ppage){
	int seek = fseek(swap_file, vpage*PAGE_SIZE,SEEK_SET);
	if(seek == -1)
		return -1;
	int write = fwrite((void*)((int)physical_memory + ppage * PAGE_SIZE), 1 ,PAGE_SIZE, swap_file);
	if(write == -1)
		return -1;
	return 0;
}

 char fetch_from_swap( int vpage,  int ppage){
	//memset((void*)(ppage*PAGE_SIZE),0, PAGE_SIZE);	retire ca marche sinon output invalide 
	int seek = fseek(swap_file, vpage*PAGE_SIZE,SEEK_SET);
	if(seek == -1)
		return -1;
	int read = fread((void*)((int)physical_memory + ppage * PAGE_SIZE),1, PAGE_SIZE , swap_file);
	if(read == -1)
		return -1;
	return 0;
}


