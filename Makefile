# $Id: Makefile,v 1.2 2009/11/17 10:02:37 marquet Exp $
##############################################################################
ROOTDIR=/home/enseign/ASE

CC	= gcc
CFLAGS	= -Wall -W -m32 -pthread -g
LIBDIR  = $(ROOTDIR)/lib 
INCDIR  = -I$(ROOTDIR)/include 
LIBS    = -L$(LIBDIR) -lhardware

###------------------------------
### Main targets
###------------------------------------------------------------
BINARIES= mi_kernel mmu_manager oracle
OBJECTS	= $(addsuffix .o,\
	  mi_kernel mi_user mmu_manager matrix swap user_process)

all: $(BINARIES) $(OBJECTS)


###------------------------------
### Binaries
###------------------------------------------------------------
mi_kernel : mi_kernel.o mi_user.o

mmu_manager : mmu_manager.o matrix.o user_process.o

oracle: oracle.o matrix.o

% : %.o
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

###------------------------------
### #include dependences
###------------------------------------------------------------
# you may fill these lines with "make depend"


%.o: %.c
	$(CC) $(CFLAGS) -c $< $(INCDIR)

###------------------------------
### Misc.
###------------------------------------------------------------
.PHONY: clean realclean depend
clean:
	$(RM) *.o $(BINARIES)
realclean: clean diskclean

diskclean:
	$(RM) vdiskA.bin vdiskB.bin
depend :
	$(CC) $(CFLAGS) -MM $(INCDIR) *.c
