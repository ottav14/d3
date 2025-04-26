TARGET = d3
SRC = main.cpp Triangle.cpp
CC = g++
LIBS = -lSDL3

all:
	$(CC) -o $(TARGET) $(SRC) $(LIBS)
	
run:
	make && ./$(TARGET)

clean:
	rm -rf $(TARGET)
