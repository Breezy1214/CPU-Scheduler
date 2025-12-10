
# Compiler and flags
CXX := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -Wpedantic -Iinclude
LDFLAGS := 
LIBS := -pthread

# Optimization flags
RELEASE_FLAGS := -O3 -DNDEBUG
DEBUG_FLAGS := -g -O0 -DDEBUG

# Directories
SRC_DIR := src
INCLUDE_DIR := include
BUILD_DIR := build
BIN_DIR := bin
TEST_DIR := test
DOC_DIR := doc
SCRIPTS_DIR := scripts

# Output executable
TARGET := $(BIN_DIR)/scheduler
TARGET_DEBUG := $(BIN_DIR)/scheduler_debug
TEST_TARGET := $(BIN_DIR)/test_runner

# Source files
SOURCES := $(wildcard $(SRC_DIR)/*.cpp)
HEADERS := $(wildcard $(INCLUDE_DIR)/*.h)
OBJECTS := $(SOURCES:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)
OBJECTS_DEBUG := $(SOURCES:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%_debug.o)

# Test files
TEST_SOURCES := $(wildcard $(TEST_DIR)/*.cpp)
TEST_OBJECTS := $(TEST_SOURCES:$(TEST_DIR)/%.cpp=$(BUILD_DIR)/test_%.o)

# Colors for output
RED := \033[0;31m
GREEN := \033[0;32m
YELLOW := \033[0;33m
BLUE := \033[0;34m
NC := \033[0m # No Color

# ============================================================================
# Main Targets
# ============================================================================

.PHONY: all build debug test clean install help

# Default target
all: build

# Standard optimized build
build: CXXFLAGS += $(RELEASE_FLAGS)
build: directories $(TARGET)
	@echo "$(GREEN)✓ Build complete: $(TARGET)$(NC)"

# Debug build with symbols
debug: CXXFLAGS += $(DEBUG_FLAGS)
debug: directories $(TARGET_DEBUG)
	@echo "$(GREEN)✓ Debug build complete: $(TARGET_DEBUG)$(NC)"

# Build and run tests
test: directories $(TEST_TARGET)
	@echo "$(BLUE)Running unit tests...$(NC)"
	@$(TEST_TARGET)
	@echo "$(GREEN)✓ All tests passed$(NC)"

# Clean build artifacts
clean:
	@echo "$(YELLOW)Cleaning build artifacts...$(NC)"
	@rm -rf $(BUILD_DIR)/* $(BIN_DIR)/*
	@echo "$(GREEN)✓ Clean complete$(NC)"

# Install to system (prepares for deployment)
install: build
	@echo "$(BLUE)Installing CPU Scheduler Simulator...$(NC)"
	@mkdir -p $(HOME)/bin
	@cp $(TARGET) $(HOME)/bin/scheduler
	@chmod +x $(HOME)/bin/scheduler
	@echo "$(GREEN)✓ Installation complete: $(HOME)/bin/scheduler$(NC)"
	@echo "$(YELLOW)Add $(HOME)/bin to your PATH if not already included$(NC)"

# Display help information
help:
	@echo "$(BLUE)CPU Scheduler Simulator - Build System$(NC)"
	@echo ""
	@echo "Available targets:"
	@echo "  $(GREEN)build$(NC)     - Compile optimized release version"
	@echo "  $(GREEN)debug$(NC)     - Compile with debug symbols (-g)"
	@echo "  $(GREEN)test$(NC)      - Build and run unit tests"
	@echo "  $(GREEN)clean$(NC)     - Remove all build artifacts"
	@echo "  $(GREEN)install$(NC)   - Install executable to ~/bin"
	@echo "  $(GREEN)run$(NC)       - Build and run the simulator"
	@echo "  $(GREEN)benchmark$(NC) - Run performance benchmarks"
	@echo "  $(GREEN)docs$(NC)      - Generate API documentation"
	@echo "  $(GREEN)format$(NC)    - Format source code with clang-format"
	@echo "  $(GREEN)lint$(NC)      - Run static analysis"
	@echo "  $(GREEN)help$(NC)      - Show this help message"

# ============================================================================
# Build Rules
# ============================================================================

# Link release executable
$(TARGET): $(OBJECTS) $(BUILD_DIR)/main.o
	@echo "$(BLUE)Linking $@...$(NC)"
	@$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS) $(LIBS)

# Link debug executable
$(TARGET_DEBUG): $(OBJECTS_DEBUG) $(BUILD_DIR)/main_debug.o
	@echo "$(BLUE)Linking $@...$(NC)"
	@$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS) $(LIBS)

# Link test executable
$(TEST_TARGET): $(filter-out $(BUILD_DIR)/main.o, $(OBJECTS)) $(TEST_OBJECTS)
	@echo "$(BLUE)Linking test suite...$(NC)"
	@$(CXX) $(CXXFLAGS) $(DEBUG_FLAGS) $^ -o $@ $(LDFLAGS) $(LIBS)

# Compile release object files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp $(HEADERS)
	@echo "$(BLUE)Compiling $<...$(NC)"
	@$(CXX) $(CXXFLAGS) -c $< -o $@

# Compile main.o separately
$(BUILD_DIR)/main.o: src/main.cpp $(HEADERS)
	@echo "$(BLUE)Compiling $<...$(NC)"
	@$(CXX) $(CXXFLAGS) -c $< -o $@

# Compile debug object files
$(BUILD_DIR)/%_debug.o: $(SRC_DIR)/%.cpp $(HEADERS)
	@echo "$(BLUE)Compiling $< (debug)...$(NC)"
	@$(CXX) $(CXXFLAGS) -c $< -o $@

# Compile main_debug.o separately
$(BUILD_DIR)/main_debug.o: src/main.cpp $(HEADERS)
	@echo "$(BLUE)Compiling $< (debug)...$(NC)"
	@$(CXX) $(CXXFLAGS) -c $< -o $@

# Compile test object files
$(BUILD_DIR)/test_%.o: $(TEST_DIR)/%.cpp $(HEADERS)
	@echo "$(BLUE)Compiling test $<...$(NC)"
	@$(CXX) $(CXXFLAGS) $(DEBUG_FLAGS) -c $< -o $@

# ============================================================================
# Utility Targets
# ============================================================================

# Create necessary directories
.PHONY: directories
directories:
	@mkdir -p $(BUILD_DIR) $(BIN_DIR)

# Build and run
.PHONY: run
run: build
	@echo "$(GREEN)Running CPU Scheduler Simulator...$(NC)"
	@$(TARGET)

# Run with debug
.PHONY: run-debug
run-debug: debug
	@echo "$(GREEN)Running CPU Scheduler Simulator (debug)...$(NC)"
	@gdb $(TARGET_DEBUG)

# Run performance benchmarks
.PHONY: benchmark
benchmark: build
	@echo "$(BLUE)Running performance benchmarks...$(NC)"
	@$(TARGET) --benchmark

# Generate documentation with Doxygen
.PHONY: docs
docs:
	@echo "$(BLUE)Generating API documentation...$(NC)"
	@doxygen Doxyfile 2>/dev/null || echo "$(YELLOW)Warning: Doxygen not configured$(NC)"
	@echo "$(GREEN)✓ Documentation generated in $(DOC_DIR)/html$(NC)"

# Format source code
.PHONY: format
format:
	@echo "$(BLUE)Formatting source code...$(NC)"
	@find $(SRC_DIR) $(INCLUDE_DIR) $(TEST_DIR) -name "*.cpp" -o -name "*.h" | \
		xargs clang-format -i --style=file 2>/dev/null || \
		echo "$(YELLOW)Warning: clang-format not installed$(NC)"
	@echo "$(GREEN)✓ Code formatted$(NC)"

# Run static analysis
.PHONY: lint
lint:
	@echo "$(BLUE)Running static analysis...$(NC)"
	@cppcheck --enable=all --suppress=missingIncludeSystem \
		--std=c++17 -I$(INCLUDE_DIR) $(SRC_DIR) 2>/dev/null || \
		echo "$(YELLOW)Warning: cppcheck not installed$(NC)"

# Check for memory leaks
.PHONY: valgrind
valgrind: debug
	@echo "$(BLUE)Running valgrind memory check...$(NC)"
	@valgrind --leak-check=full --show-leak-kinds=all $(TARGET_DEBUG)

# Coverage analysis
.PHONY: coverage
coverage: CXXFLAGS += --coverage
coverage: LDFLAGS += --coverage
coverage: clean test
	@echo "$(BLUE)Generating coverage report...$(NC)"
	@gcov $(SOURCES)
	@lcov --capture --directory . --output-file coverage.info
	@genhtml coverage.info --output-directory coverage
	@echo "$(GREEN)✓ Coverage report generated in coverage/$(NC)"

# ============================================================================
# Dependencies
# ============================================================================

# Include auto-generated dependencies
-include $(OBJECTS:.o=.d)
-include $(OBJECTS_DEBUG:.o=.d)

# Generate dependencies automatically
$(BUILD_DIR)/%.d: $(SRC_DIR)/%.cpp
	@$(CXX) $(CXXFLAGS) -MM -MT '$(BUILD_DIR)/$*.o' $< -MF $@

# ============================================================================
# Info
# ============================================================================

.PHONY: info
info:
	@echo "$(BLUE)Build Configuration:$(NC)"
	@echo "  Compiler:    $(CXX)"
	@echo "  C++ Version: C++17"
	@echo "  Sources:     $(words $(SOURCES)) files"
	@echo "  Headers:     $(words $(HEADERS)) files"
	@echo "  Tests:       $(words $(TEST_SOURCES)) files"
	@echo "  Build Dir:   $(BUILD_DIR)"
	@echo "  Binary Dir:  $(BIN_DIR)"
