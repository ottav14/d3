all:
	g++ -o program main.cpp -lSDL3
	
run:
	make && ./program
