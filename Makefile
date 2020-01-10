# Project Name (executable)
PROJECT = runner
# Compiler
CC = gcc

# Run Options
COMMANDLINE_OPTIONS =

# Compiler options during compilation
COMPILE_OPTIONS =

#Header include directories
HEADERS = include
#Libraries for linking
LIBS =

# Dependency options
#DEPENDENCY_OPTIONS = -MM

#-- Do not edit below this line --

# Subdirs to search for additional source files
SUBDIRS := $(shell ls -F | grep "\/" )
DIRS := ./$(SUBDIRS)
SOURCE_FILES := $(foreach d, $(DIRS), $(shell find $(d) -name "*.c") )

# Create an object file of every cpp file

OBJECTS_FILES = $(shell find $(SOURCE_FILES) -name "*.c" -name "*.o")
OBJECTS = $(patsubst %.c, %.o, $(SOURCE_FILES)) main.o

# Dependencies
#DEPENDENCIES_FILES = $(shell find $(SOURCE_FILES) -name "*.c")
#DEPENDENCIES = $(patsubst %.c, %.d, $(SOURCE_FILES))

# Create .d files
#%.d: %.c
#		$(CC) $(DEPENDENCY_OPTIONS) $< -MT "$*.o"

# Make $(PROJECT) the default target
all: $(PROJECT) clean

$(PROJECT): $(OBJECTS)
		$(CC) -o $(PROJECT) $(OBJECTS) $(LIBS)

# Include dependencies (if there are any)
#ifneq "$(strip $(DEPENDENCIES))" ""
#  include $(DEPENDENCIES)
#endif

# Compile every cpp file to an object
%.o: %.c
		$(CC) -c $(COMPILE_OPTIONS) -o $@ $< $(HEADERS)

# Build & Run Project
run: $(PROJECT)
		./$(PROJECT) $(COMMANDLINE_OPTIONS)

# Clean & Debug
.PHONY: makefile-debug
makefile-debug:

.PHONY: clean
clean:
		rm -f $(OBJECTS)

clean-all: clean
