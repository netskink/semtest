
# common/apps/semtest directory makefile

ifeq ( $(KERNELVER),"KERNEL24" )
all: semremove semtest1 semtest2 ipcrm semlock semunlock

semremove: semremove.c semgen.o
	echo "Build semremove."
	$(CC) $(DEBUGMODE) $(KERNELVER) $(SBC) $(APPM) $(PPCMODE) $(MYINCS) $@.c $(MYLIBS) -o $@ 



semtest1: semtest1.c semgen.o
	echo "Build semtest1."
	$(CC) $(DEBUGMODE) $(KERNELVER) $(SBC) $(APPM) $(PPCMODE) $(MYINCS) $@.c $(MYLIBS) -o $@ 


semtest2: semtest2.c semgen.o
	echo "Build semtest2."
	$(CC) $(DEBUGMODE) $(KERNELVER) $(SBC) $(APPM) $(PPCMODE) $(MYINCS) $@.c $(MYLIBS) -o $@ 


semlock: semlock.c semgen.o
	echo "Build semlock."
	$(CC) $(DEBUGMODE) $(KERNELVER) $(SBC) $(APPM) $(PPCMODE) $(MYINCS) $@.c $(MYLIBS) -o $@ 


semunlock: semunlock.c semgen.o
	echo "Build semunlock."
	$(CC) $(DEBUGMODE) $(KERNELVER) $(SBC) $(APPM) $(PPCMODE) $(MYINCS) $@.c $(MYLIBS) -o $@ 




ipcrm: ipcrm.c
	echo "Build ipcrm."
	$(CC) $(DEBUGMODE) $(KERNELVER) $(SBC) $(APPM) $(PPCMODE) $(MYINCS) $@.c $(MYLIBS) -o $@ 


semgen.o: semgen.c semgen.h 
	echo "Build semgen."
	$(CC) $(DEBUGMODE) $(KERNELVER) $(SBC) $(APPM) $(PPCMODE) $(MYINCS) $@.c $(MYLIBS) -o $@ 



endif

#.PHONY: clean
clean:
	$(RM) -rf *.o *~ core $(all) objchk*



