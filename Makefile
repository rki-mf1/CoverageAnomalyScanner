TARGET = tdf
BUILD_DIR = ./build
SRC_DIR = ./src

SRCS := $(shell find $(SRC_DIR) -type f -name *.cpp)
OBJS := $(patsubst $(SRC_DIR)/%,$(BUILD_DIR)/%,$(SRCS:.cpp=.o))

# Compiler
CXX = g++ -std=c++14
CC = $(CXX)

# Date and version number from git
DATE := on $(shell git log --pretty=format:"%cd" --date=iso | cut -f 1,2 -d " " | head -n 1)
VERSION := 0.1.0-$(shell git log --pretty=format:"%h" --date=iso | head -n 1)
CXXFLAGS += -DDATE=\""$(DATE)"\" -DVERSION=\""$(VERSION)"\"

# Compiler flags
CXXFLAGS += -W -Wall -pedantic
CXXFLAGS += -march=native

# Linker flags
LDLIBS += htslib/libhts.a
LDLIBS += dlib/dlib/all/source.cpp -DDLIB_NO_GUI_SUPPORT
LDLIBS += -lz -lpthread -llzma -lbz2 -lcurl

# DEBUG   build
#CXXFLAGS += -g -pg -O0 -DDEBUG

# RELEASE build
CXXFLAGS += -O3
CXXFLAGS += -I htslib/ -I dlib/

.PHONY: all
all: $(TARGET)

.PHONY: print
print: CXXFLAGS += -DPRINT
print: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $@ $(LDLIBS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp $(SRC_DIR)/%.h
	$(CXX) $(CXXFLAGS) $(TOOLS) -c $< -o $@

.PHONY: clean
clean:
	rm -f $(OBJS) $(TARGET)