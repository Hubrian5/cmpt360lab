CC = gcc
CFLAGS = -Wall -std=c11
LIBS = -lcairo

.PHONY: all clean tar run tar

all: procSim

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
	echo "5, 2" > ../newProc/slow.txt
	echo "3, 4" > ../newProc/fast.txt
	echo "4, 5" >../newProc/medium.txt

.PHONY = clean tar

clean:
	rm -f test_dStruct *.o procSim

tar:
	@(cd ../.. && tar -zcvf pSimulator.tar.gz pSimulator)

procSimRR: dStruct.c procSimRR.c
	$(CC) $(CFLAGS) $^ -o $@ $(LIBS)
procSimSJF: dStruct.c procSimSJF.c
	$(CC) $(CFLAGS) $^ -o $@ $(LIBS)

procSimFIFO: dStruct.c procSimFIFO.c
	$(CC) $(CFLAGS) $^ -o $@ $(LIBS)


test:
	@ls ../newProc
	@make newProc
	@ls ../newProc
	@make procSimRR
	@./procSimRR
	@make procSimSJF
	@./procSimSJF
	@make procSimFIFO
	@./procSimFIFO
	@ls ../newProc
	python3 plotData.py ../log/"log-RR.txt"
	python3 plotData.py ../log/"log-SJF.txt"
	python3 plotData.py ../log/"log-FIFO.txt"



run:
	@echo "Before newProc:"
	@ls ../newProc
	@make newProc
	@echo "After newProc:"
	@ls ../newProc
	@make procSim
	@./procSim
	@echo "After procSim has run:"
	@ls ../newProc
	@echo "Log content:"
	@cat ../log/log-`date +'%m-%d-%y'`.txt
