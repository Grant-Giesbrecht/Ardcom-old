CC = clang++ -std=c++11

all: nRF_two_way_1.cpp Ardcom.o stdutil.o string_manip.o
	$(CC) -o nrf nRF_two_way_1.cpp Ardcom.o stdutil.o string_manip.o

Ardcom.o: Ardcom.cpp Ardcom.hpp
	$(CC) -c Ardcom.cpp

stdutil.o: stdutil.cpp
	$(CC) -c stdutil.cpp

string_manip.o: string_manip.cpp
	$(CC) -c string_manip.cpp

clean:
	rm *.o

