XERCES_PATH = /usr/xerces/
CXX = g++
CXXFLAGS = -std=c++17 -O3 -Wall -I ./include -I ./include/entities -I ./include/readers -I ./include/exporters -I $(XERCES_PATH)include/
XERCES_FLAGS = -L $(XERCES_PATH)lib -lxerces-c

SRC_DIR = src
OBJ_DIR = obj

SRCS = $(wildcard $(SRC_DIR)/**/*.cpp) $(wildcard $(SRC_DIR)/*.cpp)
OBJS = $(patsubst $(SRC_DIR)/**/%.cpp, $(OBJ_DIR)/%.o, $(SRCS))
TARGET = redatam

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(XERCES_FLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -o $@ $<

clean:
	rm -rf $(OBJ_DIR) $(TARGET)

.PHONY: clean
