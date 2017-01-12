#ifndef MI_KERNEL_H
#define MI_KERNEL_H

struct tlb_entry_s{
  unsigned rfu:8;
  unsigned vpage:12;
  unsigned ppage:8;
  unsigned exec:1;
  unsigned write:1;
  unsigned read:1;
  unsigned used:1;
};

static unsigned current_process = 0;

static void switch_to_process0();
static void switch_to_process1();
static int ppage_of_vadrr(int process, unsigned vadrr);
static void mmu_handler();
void init_kernel();

#endif 
