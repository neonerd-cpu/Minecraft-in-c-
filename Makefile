CXX = g++
CXXFLAGS = -Iinclude -Wall -Wextra

all:
	mkdir -p build
	$(CXX) src/main.cpp src/game.cpp src/shader.cpp src/chunk.cpp src/world.cpp src/selection_box.cpp src/crosshair.cpp src/player.cpp src/ui.cpp src/inventory.cpp $(CXXFLAGS) -o build/app -lSDL2 -lepoxy -lGL

run:
	./build/app

clean:
	rm -rf build
