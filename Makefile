# Compiler and Flags
CXX      = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Iinclude
LDFLAGS  = -lws2_32

# Output Executable Name
TARGET   = app.exe

# Directories
SRC_DIR  = src
INC_DIR  = include

# Automatically locate all .cpp files in src/
SRCS     = $(wildcard $(SRC_DIR)/*.cpp)
OBJS     = $(SRCS:.cpp=.o)

# Default Rule
all: $(TARGET)

# Link Object Files into Executable
$(TARGET): $(OBJS)
	$(CXX) $(OBJS) -o $(TARGET) $(LDFLAGS)

# Compile .cpp files into .o object files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean Build Artifacts (Windows Command)
clean:
	del /Q /F src\*.o $(TARGET) 2>NUL || true

.PHONY: all clean