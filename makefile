CXX := g++
CXXFLAGS := -std=c++20 -O2 -Wall -Wextra -Wpedantic -Iinclude
LDFLAGS :=

BIN := build/concurrent_kv
SRC := $(wildcard src/*.cpp)

all: $(BIN)

$(BIN): $(SRC) | build
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

build:
	mkdir -p build

run: $(BIN)
	./$(BIN)

clean:
	rm -rf build

.PHONY: all run clean
