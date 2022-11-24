TARGET=cas
BUILD_DIR=./build
SRC_DIR=./src

SRCS=$(shell find $(SRC_DIR) -type f -name *.cpp)
OBJS=$(patsubst $(SRC_DIR)/%,$(BUILD_DIR)/%,$(SRCS:.cpp=.o))

# Compiler
CXX=g++ -std=c++17
CC=$(CXX)

# Compiler flags
CXXFLAGS=-W -Wall -pedantic -march=native

# Additional libraries for compilation of all builds types
CXXFLAGS+=-I htslib/
CXXFLAGS+=-I argparse/include/

# Libraries for the linker
LDLIBS=htslib/libhts.a
LDLIBS+=-lz -lpthread -llzma -lbz2 -lcurl

# Date and version
DATE=on 2022-11-24
VERSION=0.2.3
CXXFLAGS+=-DDATE=\""$(DATE)"\" -DVERSION=\""$(VERSION)"\"

.PHONY: all
all: CXXFLAGS+=-O3
all: $(TARGET)

.PHONY: print
print: CXXFLAGS+=-DPRINT -O3
print: $(TARGET)

.PHONY: debug
debug: CXXFLAGS+=-g -pg -O0 -DDEBUG
debug: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $@ $(LDLIBS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp $(SRC_DIR)/%.h
	$(CXX) $(CXXFLAGS) $(TOOLS) -c $< -o $@

.PHONY: clean
clean:
	rm -f $(OBJS) $(TARGET)
