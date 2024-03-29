# Compiler flags go here.
CFLAGS = -g -std=c99 -O2 -I . -MMD

# Linker flags go here.
LDFLAGS = -lpthread -lcomedi -g -lm

# list of sources
ELEVSRC = elev.c io.c main.c ko.c heisstyring.c timer.c

# program executable file name.
TARGET = heis

# top-level rule, to compile everything.
all: $(TARGET)

# Define dependencies
DEPS = $(shell find -name "*.d")

# Define all object files.
ELEVOBJ = $(ELEVSRC:.c=.o)

# rule to link the program
$(TARGET): $(ELEVOBJ)
	gcc $^ -o $@ $(LDFLAGS)

# Compile: create object files from C source files.
%.o : %.c
	gcc $(CFLAGS) -c  $< -o $@ 

# Include dependencies, if any.
-include $(DEPS) foo

# rule for cleaning re-compilable files.
clean:
	rm -f $(TARGET) $(ELEVOBJ) $(DEPS)

rebuild:	clean all

.PHONY: all rebuild clean


#Martin Korsgaard, 2006
