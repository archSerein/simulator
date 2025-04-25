# Binary name
BIN := cachesim

# Source and include paths
SRC_DIR := ./src
INC_DIR := ./include

# Source and object files
SRCS := $(shell find $(SRC_DIR) -name "*.cpp")
OBJS := $(SRCS:%.cpp=%.o)

# Compiler and flags
CXX := g++
CXXFLAGS := -std=c++11 -Wall -O2 -DCACHETYPE=\"$(mainargs)\"
INCFLAGS := -I$(INC_DIR)
LDFLAGS ?=

PATH_ARG ?=

# Build target
$(BIN): $(OBJS)
	@echo $(CXXFLAGS)
	$(CXX) $(CXXFLAGS) $(INCFLAGS) -o $@ $^ $(LDFLAGS)

# Compile source files into object files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCFLAGS) -c $< -o $@

# Run the simulator
run: $(BIN)
	./$(BIN) $(PATH_ARG)
	@echo "### SIMULATION DONE ###"

# Clean build files
clean:
	rm -f $(BIN) $(OBJS)

.PHONY: run clean
