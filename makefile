# -----------------------------------------------------------------------------
# Compiler and Linker Settings
# -----------------------------------------------------------------------------
CXX        := g++
CXXFLAGS   := -Wall -Wextra -std=c++17 -g # -g for debugging, -O2 for release
LDFLAGS    :=
LDLIBS     :=  -lgdi32 -luser32 -lkernel32 -lshell32 -lcomctl32 -lole32 -ld2d1	
# e.g., -lm -lpthread

# -----------------------------------------------------------------------------
# Project Structure
# -----------------------------------------------------------------------------
TARGET_EXEC := my_program

SRC_DIR     := src
OBJ_DIR     := obj
BIN_DIR     := bin
INCLUDE_DIR := include

# Add include directory to compiler flags
CXXFLAGS += -I$(INCLUDE_DIR)

# -----------------------------------------------------------------------------
# Automatic File Discovery
# -----------------------------------------------------------------------------
# Find all .cpp files in SRC_DIR
# $(wildcard pattern) searches for files matching pattern
SRC_FILES := $(wildcard $(SRC_DIR)/*.cpp)

# Generate object file names from source file names
# $(patsubst pattern,replacement,text)
# Example: src/main.cpp -> obj/main.o
OBJ_FILES := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRC_FILES))

# Generate dependency file names (for header dependencies)
# Example: obj/main.o -> obj/main.d
DEP_FILES := $(OBJ_FILES:.o=.d)

# Full path to the executable
EXECUTABLE := $(BIN_DIR)/$(TARGET_EXEC)

# -----------------------------------------------------------------------------
# Main Targets
# -----------------------------------------------------------------------------
.PHONY: all clean fclean re

all: $(EXECUTABLE)

# -----------------------------------------------------------------------------
# Linking Rule: Create the executable from object files
# -----------------------------------------------------------------------------
$(EXECUTABLE): $(OBJ_FILES) | $(BIN_DIR) # $(BIN_DIR) is an order-only prerequisite
	@echo "Linking $@..."
	$(CXX) $(LDFLAGS) $^ -o $@ $(LDLIBS)

# -----------------------------------------------------------------------------
# Compilation Rule: Compile .cpp files into .o files
# And generate dependency files (.d)
# -----------------------------------------------------------------------------
# $@: target file name (e.g., obj/main.o)
# $<: first prerequisite name (e.g., src/main.cpp)
# -MMD: Generate dependency info, but exclude system headers
# -MP:  Create phony targets for prerequisites. Avoids errors if a header is deleted.
# -MF $@.d: No, we want the .d file to have the same base name as the .o file.
#           The compiler will  output the .d file based on the .o file name if -MF is not used correctly.
#           A common practice is to name the dependency file based on the target object file.
#           So if the target is obj/main.o, the dep file is obj/main.d.
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR) # $(OBJ_DIR) is an order-only prerequisite
	@echo "Compiling $< -> $@..."
	$(CXX) $(CXXFLAGS) -MMD -MP -c $< -o $@

# -----------------------------------------------------------------------------
# Directory Creation Rules (Order-only prerequisites)
# -----------------------------------------------------------------------------
# These targets ensure the directories exist before files are placed in them.
$(OBJ_DIR):
	@mkdir -p $@

$(BIN_DIR):
	@mkdir -p $@

# -----------------------------------------------------------------------------
# Include Dependency Files
# -----------------------------------------------------------------------------
# -include: Include the files, but don't complain if they don't exist (e.g., first build)
-include $(DEP_FILES)

# -----------------------------------------------------------------------------
# Cleaning Rules
# -----------------------------------------------------------------------------
clean:
	@echo "Cleaning object files and dependency files..."
	rm -rf $(OBJ_DIR)

fclean: clean
	@echo "Cleaning executable..."
	rm -f $(EXECUTABLE)
	@echo "Cleaning bin directory if empty..."
	@rmdir $(BIN_DIR) 2>/dev/null || true # Try to remove, ignore error if not empty or doesn't exist

re: fclean all

# -----------------------------------------------------------------------------
# Help / Debug
# -----------------------------------------------------------------------------
.PHONY: info
info:
	@echo "--- Project Information ---"
	@echo "Source files: $(SRC_FILES)"
	@echo "Object files: $(OBJ_FILES)"
	@echo "Dependency files: $(DEP_FILES)"
	@echo "Executable:   $(EXECUTABLE)"
	@echo "CXXFLAGS:     $(CXXFLAGS)"
	@echo "LDFLAGS:      $(LDFLAGS)"
	@echo "LDLIBS:       $(LDLIBS)"
