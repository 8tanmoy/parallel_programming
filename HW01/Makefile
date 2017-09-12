# Warnings
WFLAGS	:= -Wall -Wextra -Wsign-conversion -Wsign-compare

# Optimization and architecture
OPT		:= -O3
ARCH   	:= -march=native

# Language standard
CCSTD	:= -std=c99
CXXSTD	:= -std=c++11

# Linker options
LDOPT 	:= $(OPT)
LDFLAGS := 

.DEFAULT_GOAL := all

.PHONY: debug
debug : OPT  := -O0 -g -fno-omit-frame-pointer -fsanitize=address
debug : LDFLAGS := -fsanitize=address
debug : ARCH :=
debug : $(EXEC)

EXEC1    := problem1
EXEC2	:= problem2

all : Makefile $(EXEC1).exe $(EXEC2).exe 

$(EXEC1).o: $(EXEC1).c
	@ echo Compiling $<...
	@ $(CC) $(CCSTD) $(WFLAGS) $(OPT) $(ARCH) $(CFLAGS) -c $< -o $@

$(EXEC1).exe: $(EXEC1).o
	@ echo Building $@...
	@ $(CC) -o $@ $< $(LDFLAGS) $(LDFLAGS)

$(EXEC2).o: $(EXEC2).c
	@ echo Compiling $<...
	@ $(CC) $(CCSTD) $(WFLAGS) $(OPT) $(ARCH) $(CFLAGS) -c $< -o $@

$(EXEC2).exe: $(EXEC2).o
	@ echo Building $@...
	@ $(CC) -o $@ $< $(LDFLAGS) $(LDFLAGS)

%.o : %.cpp Makefile
	@ echo Compiling $<...
	$(CXX) $(CXXSTD) $(WFLAGS) $(OPT) $(ARCH) $(CXXFLAGS) -c $< -o $@

# TODO: add targets for building executables

.PHONY: clean
clean:
	@ rm -f *.o *.exe
