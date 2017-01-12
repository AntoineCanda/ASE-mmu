#include "mmu_manager.h"

static void mmu_handler_unique(){

	struct tlb_entry_s tlbent;
	
	// On recupere l'adresse qui a cause le signal et on en recupere le numero de page
	unsigned int vaddr = _in(MMU_FAULT_ADDR);
	unsigned int vpage = (vaddr >> 12) & 0xfff ;

	if(vpage_courant != -1){

		// On met dans le swap la page virtuelle courante
		store_to_swap(vpage_courant, 1);

		tlbent.ppage = 1;
		/*tlbent.vpage = vpage_courant;
		tlbent.exec = 1;
		tlbent.read = 1;
		tlbent.write = 1;
		tlbent.used = 1;
		tlbent.rfu = 0;*/

		// On retire son entree dans la TLB
		_out(TLB_DEL_ENTRY, *(int*)(&tlbent));

		// On recupere la page depuis le swap 
		fetch_from_swap(vpage,1);
	}

	// On met a jour la page virtuelle courante 
	vpage_courant = vpage;


	// On met a jour la TLB
	tlbent.ppage = 1;
	tlbent.vpage = vpage;
	tlbent.exec = 1;
	tlbent.read = 1;
	tlbent.write = 1;
	tlbent.used = 1;
	tlbent.rfu = 0;
	_out(TLB_ADD_ENTRY, *(int*)(&tlbent));

	return;
}

void mmu_handler_complet(){
	
	// On recupere l'adresse et on en determine la page
	unsigned int vaddr = _in(MMU_FAULT_ADDR);
	unsigned int vpage = (vaddr >> 12) & 0xfff;

	// La page est en memoire physique mais pas dans la TLB : on l'y ajoute
	if(vm_mapping[vpage].allocate == 1){

		tlbent.ppage = vm_mapping[vpage].ppage;
		tlbent.vpage = vpage;
		tlbent.exec = 1;
		tlbent.read = 1;
		tlbent.write = 1;
		tlbent.used = 1;
		tlbent.rfu = 0;

		_out(TLB_ADD_ENTRY, *(int*)(&tlbent));
	}
	else{

	/* La page n'est pas en memoire physique donc on va l'y ajouter et on va pour cela mettre une page en swap, recuperer celle de la page, mettre a jour la tlb ... */
		if(cpt_ppage == PM_PAGES){
			cpt_ppage = 1;
		}

		if(pm_mapping[cpt_ppage].allocate == 1){

		// 1
			store_to_swap(pm_mapping[cpt_ppage].vpage, cpt_ppage);
		
		// 2
			tlbent.ppage = cpt_ppage;
			tlbent.vpage = pm_mapping[cpt_ppage].vpage;
			tlbent.exec = 1;
			tlbent.read = 1;
			tlbent.write = 1;
			tlbent.used = 1;
			tlbent.rfu = 0;

			_out(TLB_DEL_ENTRY, *(int*)(&tlbent));

			pm_mapping[cpt_ppage].allocate = 0;
			vm_mapping[pm_mapping[cpt_ppage].vpage].allocate = 0;

		}

		// mise a jour de la tlb avec ajout de la nouvelle page 
		tlbent.ppage = cpt_ppage;
		tlbent.vpage = vpage;
		tlbent.exec = 1;
		tlbent.read = 1;
		tlbent.write = 1;
		tlbent.used = 1;
		tlbent.rfu = 0;

		_out(TLB_ADD_ENTRY, *(int*)(&tlbent));

		// maj tableau

		vm_mapping[vpage].ppage = cpt_ppage;
		vm_mapping[vpage].allocate = 1;

		pm_mapping[cpt_ppage].allocate = 1;
		pm_mapping[cpt_ppage].vpage = vpage;
		
		// incremente compteur ppage
		cpt_ppage++;
	}
	return;	
}

// Initialisation du tableau contenant les structures pm_mapping
static void init_tab_pm_mapping(struct pm_mapping_s tab[], int taille){
	int i=0;
	for(i = 0; i< taille; i++){
		tab[i].vpage = VM_PAGES;
		tab[i].allocate = 0;
	}
	return;
}

// Initialisation du tableau contenant les structures vm_mapping
static void init_tab_vm_mapping(struct vm_mapping_s tab[], int taille){
	int i=0;
	for(i = 0; i< taille; i++){
		tab[i].ppage = PM_PAGES;
		tab[i].allocate = 0;
	}
	return;
}

// Initialisation du kernel avec en fonction de mmu, le handler associe
static void init_kernel(int mmu) {
  if(init_hardware("hardware.ini") == 0) {
    fprintf(stderr, "Error in hardware initialization\n");
    exit(EXIT_FAILURE);
  }
  
  if(init_swap("swap_file") == -1) {
    fprintf(stderr, "Error in swap initialization\n");
    exit(EXIT_FAILURE);
  }

  if(mmu == HANDLER_UNIQUE){
	IRQVECTOR[MMU_IRQ]= mmu_handler_unique;
  }
  else if(mmu == HANDLER_COMPLET){
	IRQVECTOR[MMU_IRQ]= mmu_handler_complet;
	init_tab_pm_mapping(pm_mapping, PM_PAGES);
	init_tab_vm_mapping(vm_mapping, VM_PAGES);
  }

  _mask(0x1001);

return;
}
 





int main(int argc, char **argv) {
	

// Handler par defaut : HANDLER_COMPLET
int mmu = 0;
// Operation par defaut : addition de matrices
int operation = 0;

if(argc == 3){
	operation = atoi(argv[2]);
	if(operation != MATRIX_ADD && operation != MATRIX_MUL){
		fprintf(stderr, "La valeur de l'argument passé en 2e paramètre doit être 0 ou 1.");
	}
	mmu = atoi(argv[1]);
	if(mmu != 0 && mmu !=1){
		fprintf(stderr,"La valeur de l'argument passé en 1er parametre doit etre 0 ou 1.");
	}

}
	

init_kernel(mmu);
user_process(operation);

	return EXIT_SUCCESS;
}

