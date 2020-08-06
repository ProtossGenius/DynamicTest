

main.o: main.cpp .//actions//smdtest/smncpp/lockm.h .//actions/smdtest/action.h .//smdtest//smncpp/lockm.h .//smdtest/action.h .//smdtest/business.h .//smdtest/process.h .//smdtest/smncpp/channel.h .//smdtest/smncpp/ticker.h .//smdtest/strategy.h ./actions.h \
./actions/login_action.h ./smdtest/user.h MyUser.h my_strategy.h
	g++ -Wall -c main.cpp
my_strategy.o: my_strategy.cpp .//actions//smdtest/smncpp/lockm.h .//actions/smdtest/action.h .//smdtest//smncpp/channel.h .//smdtest/action.h .//smdtest/business.h .//smdtest/process.h .//smdtest/smncpp/lockm.h .//smdtest/smncpp/ticker.h ./actions.h ./actions/login_action.h \
./smdtest/strategy.h my_strategy.h smdtest/user.h
	g++ -Wall -c my_strategy.cpp
sm_build: main.o my_strategy.o
	+make -C actions sm_build

sm_build_all: main.o my_strategy.o
	+make -C actions sm_build_all
	+make -C smdtest sm_build_all
	+make -C smncpp sm_build_all

sm_clean_o:
	rm -rf ./*.o
	+make -C actions sm_clean_o

##Tail
debug:

qrun: sm_build_all
	g++ ./*.o ./smdtest/*.o ./actions/*.o ./smncpp/*.o  -o run.main -lpthread
	./run.main

test:

install:

clean: sm_clean_o
	rm -rf ./run.main	

