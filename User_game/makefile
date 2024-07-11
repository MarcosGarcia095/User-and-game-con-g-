# Variables
CXX = g++
CXXFLAGS = -std=c++11 -Iinclude
LDFLAGS = -lsqlite3

# Directorios
SRC_DIR = src
BUILD_DIR = build
INCLUDE_DIR = include

# Archivos fuente
SRCS = $(wildcard $(SRC_DIR)/*.cpp)

# Archivos objeto
OBJS = $(SRCS:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)

# Nombre del ejecutable
TARGET = main

# Reglas
all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) -o $@ $^ $(LDFLAGS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -o $@ -c $<

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

clean:
	rm -rf $(BUILD_DIR) $(TARGET)

.PHONY: all clean
