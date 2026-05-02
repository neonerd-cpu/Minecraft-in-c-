CXX = g++
CXXFLAGS = -Iinclude

all:
	$(CXX) src/main.cpp src/game.cpp $(CXXFLAGS) -o build/app -lSDL2 -lGL
run:
	./build/app
