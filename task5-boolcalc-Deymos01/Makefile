CC=g++
CFLAGS=-pedantic-errors -g
SOURCES_EXECUTABLE=boolcalc.cpp boolexpr.cpp boolnode.cpp
SOURCES_TEST=main.cpp test.cpp boolexpr.cpp boolnode.cpp

all: boolcalc
    
boolcalc:
	$(CC) $(SOURCES_EXECUTABLE) $(CFLAGS) -o $@

test: 
	$(CC) $(SOURCES_TEST) $(CFLAGS) -lgtest -lgtest_main -lpthread -o $@

.PHONY: clean

clean:
#	rm -rf *.o boolcalc test
	cmd //C del *.o $(EXECUTABLE).exe
