CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2 -Iinclude

BIN_DIR = bin
OBJ_DIR = obj/src
TARGET = $(BIN_DIR)/reader_ocr

SRC = src/main.cpp src/tesseract_lib.cpp
OBJ = $(patsubst src/%.cpp,$(OBJ_DIR)/%.o,$(SRC))

$(TARGET): $(OBJ)
	mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(OBJ_DIR)/%.o: src/%.cpp
	mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) $(OBJ)

.PHONY: clean

run: $(TARGET)
	./$(TARGET)
