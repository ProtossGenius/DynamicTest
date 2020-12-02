
actions.o: actions.cpp ./actions/smdtest/action.h ./actions/smdtest/smncpp/lockm.h actions.h actions/login_action.h
	g++ -Wall -c actions.cpp
main.o: main.cpp ./actions.h ./smdtest/business.h ./smdtest/process.h ./smdtest/smncpp/channel.h ./smdtest/smncpp/lockm.h ./smdtest/smncpp/ticker.h ./smdtest/strategy.h ./smdtest/user.h MyUser.h my_strategy.h
	g++ -Wall -c main.cpp
my_strategy.o: my_strategy.cpp ./actions.h ./smdtest/business.h ./smdtest/process.h ./smdtest/smncpp/channel.h ./smdtest/smncpp/lockm.h ./smdtest/smncpp/ticker.h ./smdtest/strategy.h my_strategy.h smdtest/user.h
	g++ -Wall -c my_strategy.cpp
sm_build: actions.o main.o my_strategy.o
	+make -C actions sm_build

sm_build_all: actions.o main.o my_strategy.o
	+make -C actions sm_build_all
	+make -C smdtest sm_build_all
	+make -C smncpp sm_build_all

sm_clean_o:
	rm -rf ./*.o ./run.main
	+make -C actions sm_clean_o

##Tail
build:
	smdtcact
	+make sm_build_all 

debug:

qrun: build
	g++ ./*.o ./smdtest/*.o ./actions/*.o ./smncpp/*.o  -o run.main -lpthread
	./run.main

test:

install:

clean: sm_clean_o
	rm -rf ./run.main	

