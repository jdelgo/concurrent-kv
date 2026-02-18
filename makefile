CXX := g++
CXXFLAGS := -std=c++20 -O2 -Wall -Wextra -Wpedantic -Iinclude -pthread
LDFLAGS :=

BIN := build/concurrent_kv
BENCH_BIN := build/bench

LIB_SRC := $(wildcard src/*.cpp)
APP_SRC := $(wildcard app/*.cpp)
BENCH_SRC := $(wildcard bench/*.cpp)

all: $(BIN)

$(BIN): $(LIB_SRC) $(APP_SRC) | build
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

$(BENCH_BIN): $(LIB_SRC) $(BENCH_SRC) | build
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

build:
	mkdir -p build

run: $(BIN)
	./$(BIN)

bench: $(BENCH_BIN)
	./$(BENCH_BIN)
clean:
	rm -rf build

.PHONY: all run bench clean
