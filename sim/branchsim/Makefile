# Binary name
BIN := branchsim

# Source and include paths
BUILD_DIR := ./build
SRC_DIR := ./src
INC_DIR := ./include

# Source and object files
SRCS := $(shell find $(SRC_DIR) -name "*.cpp")
OBJS := $(SRCS:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)

# Compiler and flags
CXX := g++
CXXFLAGS = -std=c++17 -Wall -O2
ifeq ($(MODE), debug)
	CXXFLAGS += -g
endif
INCFLAGS := -I$(INC_DIR)
LDFLAGS ?=

ARGS ?=

$(shell mkdir -p $(BUILD_DIR))

# Build target
$(BUILD_DIR)/$(BIN): $(OBJS)
	$(CXX) $(CXXFLAGS) $(INCFLAGS) -o $@ $^ $(LDFLAGS)

# Compile source files into object files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) $(INCFLAGS) -c $< -o $@

# Run the simulator
run: $(BUILD_DIR)/$(BIN)
	@$(BUILD_DIR)/$(BIN) $(ARGS)
	@echo "### SIMULATION DONE ###"

# Clean build files
clean:
	rm -rf $(BUILD_DIR)

.PHONY: run clean
