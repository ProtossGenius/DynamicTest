

sm_build: 
	+make -C docs sm_build

sm_build_all: 
	+make -C docs sm_build_all
	+make -C smncpp sm_build_all

sm_clean_o:
	rm -rf ./*.o
	+make -C docs sm_clean_o

##Tail
debug:

qrun:

test:

install:

clean:

