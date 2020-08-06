

login_action.o: login_action.cpp .//smdtest/business.h .//smdtest/process.h .//smdtest/smncpp/channel.h .//smdtest/smncpp/lockm.h .//smdtest/smncpp/ticker.h .//smdtest/strategy.h .//smncpp/channel.h .//smncpp/lockm.h ./smdtest/action.h login_action.h \
smdtest/user.h smncpp/ticker.h
	g++ -Wall -c login_action.cpp
sm_build: login_action.o


sm_build_all: login_action.o
	+make -C smdtest sm_build_all
	+make -C smncpp sm_build_all

sm_clean_o:
	rm -rf ./*.o


##Tail
debug:

qrun:

test:

install:

clean:

