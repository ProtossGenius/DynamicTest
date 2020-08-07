
sm_build: 
	+make -C cmd sm_build
	+make -C docs sm_build
	+make -C smdtest sm_build
	+make -C test sm_build

sm_build_all: 
	+make -C cmd sm_build_all
	+make -C docs sm_build_all
	+make -C smdtest sm_build_all
	+make -C test sm_build_all

sm_clean_o:
	rm -rf ./*.o
	+make -C cmd sm_clean_o
	+make -C docs sm_clean_o
	+make -C smdtest sm_clean_o
	+make -C test sm_clean_o

##Tail
debug:

qrun:
	cd test && make test -j8
test: zero 
	cd test && make test -j8
install:
	cd ./cmd/smdtcact && go install # SureMoon Dyn..Test Collct Actions
clean: sm_clean_o
	+make -C test clean
zero:

init:
	smake

