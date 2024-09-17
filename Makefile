# Paths
XERCES_PATH = ./xerces/
SRC_DIR = src
GUI_DIR = gui
OBJ_DIR = obj
LIB_DIR = lib
INCLUDE_DIRS = -I ./include -I ./include/entities -I ./include/readers -I ./include/exporters -I $(XERCES_PATH)include/ -I $(GUI_DIR) -I $(OBJ_DIR)

# Compiler and linker
CXX = g++
CXXFLAGS = -std=c++17 -g -O3 -Wall -fPIC $(INCLUDE_DIRS)
LDFLAGS = -pthread

# Qt flags
QT_CXXFLAGS = $(shell pkg-config --cflags Qt5Widgets)
QT_LDFLAGS = $(shell pkg-config --libs Qt5Widgets)

# Xerces flags
XERCES_FLAGS = -L $(XERCES_PATH)lib -lxerces-c

# Source files
SRCS = $(filter-out $(SRC_DIR)/main.cpp, $(wildcard $(SRC_DIR)/**/*.cpp) $(wildcard $(SRC_DIR)/*.cpp))
OBJS = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRCS))

# GUI files
GUI_SOURCES = $(wildcard $(GUI_DIR)/*.cpp)
GUI_OBJECTS = $(patsubst $(GUI_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(GUI_SOURCES))

# UI files
UI_FILES = $(wildcard $(GUI_DIR)/*.ui)
UI_HEADERS = $(patsubst $(GUI_DIR)/%.ui, $(OBJ_DIR)/ui_%.h, $(UI_FILES))

# MOC files
MOC_HEADERS = $(wildcard $(GUI_DIR)/*.h)
MOC_SOURCES = $(patsubst $(GUI_DIR)/%.h, $(OBJ_DIR)/moc_%.cpp, $(MOC_HEADERS))
MOC_OBJECTS = $(patsubst $(OBJ_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(MOC_SOURCES))

# Targets
TARGET_REDATAM = redatam
TARGET_REDATAM_LIB = libredatam.a
TARGET_GUI = redatamgui

# Rules
all: $(TARGET_REDATAM) $(TARGET_GUI)
nogui: $(TARGET_REDATAM)

# Build redatam
$(TARGET_REDATAM): $(OBJS) $(OBJ_DIR)/main.o
	$(CXX) $(CXXFLAGS) -o $@ $^ $(XERCES_FLAGS) $(LDFLAGS)

$(OBJ_DIR)/main.o: $(SRC_DIR)/main.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

# Build redatam as a static library
$(TARGET_REDATAM_LIB): $(OBJS)
	@mkdir -p $(LIB_DIR)
	ar rcs $(LIB_DIR)/$@ $^

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(QT_CXXFLAGS) -c -o $@ $<

# Build redatamgui
$(TARGET_GUI): $(filter-out $(OBJ_DIR)/main.o, $(GUI_OBJECTS)) $(TARGET_REDATAM_LIB) $(OBJ_DIR)/gui_main.o $(UI_HEADERS) $(MOC_OBJECTS)
	$(CXX) $(filter-out $(OBJ_DIR)/main.o, $(GUI_OBJECTS)) $(OBJ_DIR)/gui_main.o $(MOC_OBJECTS) -L$(LIB_DIR) -lredatam $(LDFLAGS) $(XERCES_FLAGS) $(QT_LDFLAGS) -o $@

$(OBJ_DIR)/gui_main.o: $(GUI_DIR)/main.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(QT_CXXFLAGS) -c -o $@ $<

$(OBJ_DIR)/%.o: $(GUI_DIR)/%.cpp $(UI_HEADERS)
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(QT_CXXFLAGS) -c -o $@ $<

$(OBJ_DIR)/ui_%.h: $(GUI_DIR)/%.ui
	@mkdir -p $(dir $@)
	uic $< -o $@

$(OBJ_DIR)/moc_%.cpp: $(GUI_DIR)/%.h
	@mkdir -p $(dir $@)
	moc $(QT_CXXFLAGS) $< -o $@

$(OBJ_DIR)/moc_%.o: $(OBJ_DIR)/moc_%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(QT_CXXFLAGS) -c -o $@ $<

clean:
	rm -rf $(OBJ_DIR) $(LIB_DIR) $(TARGET_REDATAM) $(TARGET_GUI)

.PHONY: all clean nogui
