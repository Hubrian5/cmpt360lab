CC = gcc
CFLAGS = -Wall -std=c11
LIBS = -lcairo

.PHONY: all clean tar run tar

all: test_dStruct

procSim: dStruct.c procSim.c
	$(CC) $(CFLAGS) $^ -o $@ $(LIBS)

test_dStruct: dStruct.o test_dStruct.o
	$(CC) $(CFLAGS) $^ -o $@ $(LIBS)

dStruct.o: dStruct.c dStruct.h
	$(CC) $(CFLAGS) -c $< -o $@

test_dStruct.o: test_dStruct.c dStruct.h
	$(CC) $(CFLAGS) -c $< -o $@

newProc:
	mkdir -p ../newProc
	echo "5, 6" > ../newProc/slow.txt
	echo "3, 9" > ../newProc/fast.txt
	echo "4, 10" >../newProc/medium.txt

.PHONY = clean tar

clean:
	rm -f test_dStruct *.o procSim

tar:
	@(cd ../.. && tar -zcvf pSimulator.tar.gz pSimulator)

run:
	@echo "Before newProc:"
	@ls ../newProc
	@make newProc
	@echo "After newProc:"
	@ls ../newProc
	@make procSim
	@./procSim
	@echo "After procSim:"
	@ls ../newProc
	@echo "Log content:"
	@cat ../log/log-`date +'%m-%d-%y'`.txt