CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -pedantic
LDLIBS = -pthread

ARCHIVOS = main.cpp procesos.cpp hilos.cpp memoria.cpp

all: miniso

miniso: $(ARCHIVOS)
	$(CXX) $(CXXFLAGS) $(ARCHIVOS) -o miniso $(LDLIBS)

clean:
	rm -f miniso *.o

.PHONY: all clean
