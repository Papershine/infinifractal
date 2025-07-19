CXX = clang++
CXXFLAGS = -Wall -Wextra -pedantic -std=c++20 -O3 -I/opt/homebrew/Cellar/sdl2/2.32.2/include
LDFLAGS = `sdl2-config --cflags --libs`

EM = em++
EMFLAGS = -lm -O3 -std=c++20 --bind -sUSE_SDL=2 -sUSE_PTHREADS -sASYNCIFY -sPROXY_TO_PTHREAD -sPTHREAD_POOL_SIZE=navigator.hardwareConcurrency

SRC_DIR = src
INC_DIR = src
BUILD_DIR = build

SRCS = $(wildcard $(SRC_DIR)/*.cpp)
OBJS = $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(SRCS))

TARGET = infinifractal
WASM_TARGET = $(TARGET).js

all: $(TARGET)

wasm:
	$(EM) $(SRCS) -I$(INC_DIR) -o $(WASM_TARGET) $(EMFLAGS) --shell-file dist/index.html

$(TARGET): $(OBJS)
	$(CXX) $^ -o $@ $(LDFLAGS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -I$(INC_DIR) -c $< -o $@

$(BUILD_DIR):
	mkdir -p $@

clean:
	rm -rf $(BUILD_DIR) $(TARGET)

clean_wasm:
	rm -f $(WASM_TARGET) $(TARGET).wasm

.PHONY: all clean clean_wasm