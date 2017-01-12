#include "mi_syscall.h"
#include "mi_kernel.h"
#include "mi_user.h"
#include <assert.h>

static int ppage_of_vadrr(int process, unsigned vadrr){

  int vpage;

  if(vadrr < (unsigned)virtual_memory || vadrr >= (unsigned)virtual_memory + N/2 * PAGE_SIZE)
    return -1;

  vpage = vadrr >> 12;

  return vpage+1 + (N/2 * process);
}

static void switch_to_process0(void){
  current_process = 0;
  _out(MMU_CMD, MMU_RESET);
}

static void switch_to_process1(void){
  current_process = 1;
  _out(MMU_CMD, MMU_RESET);
}

static void mmu_handler(){
  struct tlb_entry_s tlbent;
  unsigned vaddr = _in(MMU_FAULT_ADDR);
  int ppage = ppage_of_vadrr(current_process, vaddr);

  assert(ppage != -1);
  
  tlbent.ppage = ppage;
  tlbent.vpage = vaddr >> 12;
  tlbent.exec = 1;
  tlbent.write = 1;
  tlbent.read = 1;
  tlbent.used = 1;
  tlbent.rfu = 0;

  _out(TLB_ADD_ENTRY, *((int*)(&tlbent)));
}

void init_kernel(){
	if(init_hardware("hardware.ini") == 0) {
	    fprintf(stderr, "Error in hardware initialization\n");
	    exit(EXIT_FAILURE);
	}
	  
	  IRQVECTOR[MMU_IRQ] = mmu_handler;
	  IRQVECTOR[SYSCALL_SWITCH_0] = switch_to_process0;
	  IRQVECTOR[SYSCALL_SWITCH_1] = switch_to_process1;

	  _mask(0x1001);
}

int main (){
 
  init_kernel();
  init_user();

return 0;

}
