all:
	g++ src/main.cpp -o build/app -lSDL2 -lGL

run:
	./build/app
