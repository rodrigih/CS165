DEPS1 = ParamChecker.h LZ.cpp
OBJ1 = LZ.o

DEPS2 = DoublyLinkedList.h EXPAND.cpp
OBJ2 = EXPAND.o

LZ.o: LZ.cpp $(DEPS1)
	g++ -std=c++11 -g -c -o $@ $<

EXPAND.o: EXPAND.cpp $(DEPS2)
	g++ -std=c++11 -g -c -o $@ $<


LZ: $(OBJ1)
	g++ -o $@ $^ 

EXPAND: $(OBJ2)
	g++ -o $@ $^ 

.PHONY: clean

clean:
	rm -f *.o
