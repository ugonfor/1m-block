LDLIBS=-lnetfilter_queue -lsqlite3

all: 1m-block
	
1m-block: main.o 1m-block.o 1m-block.hpp header/iphdr.hpp header/tcphdr.hpp
	g++   1m-block.o main.o  -lnetfilter_queue -lsqlite3 -o 1m-block

clean:
	rm 1m-block *.o
