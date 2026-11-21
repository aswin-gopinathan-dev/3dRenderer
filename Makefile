CXX := g++
CC := gcc

CXXFLAGS := -std=c++20 -Wall -Wextra -g -Iinc
CFLAGS := -Wall -Wextra -g -Iinc

LDLIBS := -lSDL2

SRC_DIR := src
BUILD_DIR := build

CPP_SOURCES := $(wildcard $(SRC_DIR)/*.cpp)
C_SOURCES := $(wildcard $(SRC_DIR)/*.c)

OBJECTS := $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(CPP_SOURCES))
OBJECTS += $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(C_SOURCES))

TARGET := $(BUILD_DIR)/main

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $@ $(LDLIBS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

cc:
	$(MAKE) clean
	bear -- $(MAKE)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -rf $(BUILD_DIR)/*

.PHONY: all clean run cc