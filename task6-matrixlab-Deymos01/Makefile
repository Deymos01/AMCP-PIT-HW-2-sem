CC=g++
CFLAGS=-c -pedantic-errors -g
LDFLAGS=
SOURCES=matrixlab.cpp biginteger.cpp fraction.cpp matrix.cpp parsing.cpp node.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=matrixlab

all: $(SOURCES) $(EXECUTABLE)
    
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf *.o $(EXECUTABLE)
#	cmd //C del *.o $(EXECUTABLE).exe
