

action.o: action.cpp ./smncpp/lockm.h action.h
	g++ -Wall -c action.cpp
business.o: business.cpp ./action.h ./smncpp/lockm.h business.h
	g++ -Wall -c business.cpp
process.o: process.cpp ./action.h ./business.h ./smncpp/lockm.h process.h
	g++ -Wall -c process.cpp
user.o: user.cpp .//smncpp/lockm.h ./action.h ./business.h ./process.h ./smncpp/channel.h ./smncpp/ticker.h ./strategy.h user.h
	g++ -Wall -c user.cpp
sm_build: action.o business.o process.o user.o


sm_build_all: action.o business.o process.o user.o
	+make -C smncpp sm_build_all

sm_clean_o:
	rm -rf ./*.o


##Tail
debug:

qrun:

test:

install:

clean: sm_clean_o

