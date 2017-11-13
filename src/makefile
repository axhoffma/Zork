CC=g++
CFLAGS=-Wall -Werror -O -std=c++11
SOURCES=Condition.cpp Container.cpp Creature.cpp Item.cpp Object.cpp Room.cpp Trigger.cpp helper.cpp main.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=../Zork
all: $(SOURCES) 
	$(CC) $(CFLAGS) $(SOURCES) -o $(EXECUTABLE)
clean:
	rm -f *.o
	rm -f Zork
