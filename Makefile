# Compiler and flags
CXX      := g++
CXXFLAGS := -Wall -Wextra -std=c++17

# Include paths
INCLUDES := -I. -Icap_sensor

# Target executable name
TARGET   := lickometer

# Source files
SRCS     := cap_sensor/cap_sensor.cpp main.cpp

# Generate object files list (.cpp -> .o)
OBJS     := $(SRCS:.cpp=.o)

# Default rule: builds the executable
all: $(TARGET)

# Link the object files to create the executable
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $@ $^

# Compile .cpp files to .o object files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

# Dedicated LSP rule: Cleans, then runs bear safely
# Run 'make lsp' to generate your compilation database
lsp: clean
	bear -- $(MAKE) all

# Clean up build files and the json config
clean:
	rm -f $(OBJS) $(TARGET) compile_commands.json

.PHONY: all clean lsp
