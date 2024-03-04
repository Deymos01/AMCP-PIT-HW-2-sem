CC=g++
CFLAGS=-c -pedantic-errors -g
LDFLAGS=
SOURCES=compress.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=compress

all: $(SOURCES) $(EXECUTABLE)
    
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	cmd //C del *.o
