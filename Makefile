

business.o: business.cpp .//action.h business.h smncpp/lockm.h
	g++ -Wall -c business.cpp
process.o: process.cpp .//.//action.h .//business.h process.h smncpp/lockm.h
	g++ -Wall -c process.cpp
sm_build: business.o process.o
	+make -C docs sm_build

sm_build_all: business.o process.o
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

clean: sm_clean_o

