CC=g++
CFLAGS=-c -pedantic-errors -g
LDFLAGS=
SOURCES=stockexchange.cpp dataBase.cpp order.cpp validator.cpp stringUtil.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=stockexchange

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	del *.o $(EXECUTABLE)