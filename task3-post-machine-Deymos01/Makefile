CC=g++
CFLAGS=-c -pedantic-errors -g
LDFLAGS=
SOURCES=main.cpp postmachine.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=postmachine

all: $(SOURCES) $(EXECUTABLE)
    
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	del *.o $(EXECUTABLE)
