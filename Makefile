TARGET = d3
SRC = src/main.cpp src/glad.c src/Prism.cpp
CC = g++
LIBS = -lSDL3 -lGL -lglm
CFLAGS = -Iinclude

all:
	$(CC) -o $(TARGET) $(SRC) $(CFLAGS) $(LIBS)
	
run:
	make && ./$(TARGET)

clean:
	rm -rf $(TARGET)
