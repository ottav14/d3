TARGET = d3
SRC = src/main.cpp src/glad.c
CC = g++
LIBS = -lSDL3 -lGL 
CFLAGS = -Iinclude

all:
	$(CC) -o $(TARGET) $(SRC) $(CFLAGS) $(LIBS)
	
run:
	make && ./$(TARGET)

clean:
	rm -rf $(TARGET)
