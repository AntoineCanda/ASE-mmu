#ifndef MMU_MANAGER_H
#define MMU_MANAGER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <hardware.h>
#include <assert.h>
#include "hw_config.h"
#include "swap.c"
#include "matrix.h"

#define HANDLER_UNIQUE 0
#define HANDLER_COMPLET 1

struct tlb_entry_s{
  unsigned int rfu:8;
  unsigned int vpage:12;
  unsigned int ppage:8;
  unsigned int exec:1;
  unsigned int write:1;
  unsigned int read:1;
  unsigned int used:1;
};

struct vm_mapping_s{
unsigned ppage:8;
unsigned allocate:1;
};

struct pm_mapping_s{
unsigned vpage: 12;
unsigned allocate:1;
};

struct tlb_entry_s tlbent;
// pour mmu_handler avec 1 page
int vpage_courant = -1;

// pour mmu_handler avec toute la memoire physique
int cpt_ppage = 1;

struct vm_mapping_s vm_mapping[VM_PAGES];
struct pm_mapping_s pm_mapping[PM_PAGES];

extern void user_process();

#endif
