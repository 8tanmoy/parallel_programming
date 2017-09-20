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

EXEC1    := problem1a
EXEC2	:= problem1b
EXEC3 := problem1c
EXEC4 := problem2a
EXEC5 := problem2b
EXEC6 := problem3a
EXEC7 := problem3b
EXEC8 := problem3c

all : Makefile $(EXEC1).exe $(EXEC2).exe $(EXEC3).exe $(EXEC4).exe $(EXEC5).exe $(EXEC6).exe $(EXEC7).exe $(EXEC8).exe

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

$(EXEC3).o: $(EXEC3).c
	@ echo Compiling $<...
	@ $(CC) $(CCSTD) $(WFLAGS) $(OPT) $(ARCH) $(CFLAGS) -c $< -o $@

$(EXEC3).exe: $(EXEC3).o
	@ echo Building $@...
	@ $(CC) -o $@ $< $(LDFLAGS) $(LDFLAGS)

$(EXEC4).o: $(EXEC4).c
	@ echo Compiling $<...
	@ $(CC) $(CCSTD) $(WFLAGS) $(OPT) $(ARCH) $(CFLAGS) -c $< -o $@

$(EXEC4).exe: $(EXEC4).o
	@ echo Building $@...
	@ $(CC) -o $@ $< $(LDFLAGS) $(LDFLAGS)

$(EXEC5).o: $(EXEC5).c
	@ echo Compiling $<...
	@ $(CC) $(CCSTD) $(WFLAGS) $(OPT) $(ARCH) $(CFLAGS) -c $< -o $@

$(EXEC5).exe: $(EXEC5).o
	@ echo Building $@...
	@ $(CC) -o $@ $< $(LDFLAGS) $(LDFLAGS)

$(EXEC6).o: $(EXEC6).c
	@ echo Compiling $<...
	@ $(CC) $(CCSTD) $(WFLAGS) $(OPT) $(ARCH) $(CFLAGS) -c $< -o $@

$(EXEC6).exe: $(EXEC6).o
	@ echo Building $@...
	@ $(CC) -o $@ $< $(LDFLAGS) $(LDFLAGS)

$(EXEC7).o: $(EXEC7).c
	@ echo Compiling $<...
	@ $(CC) $(CCSTD) $(WFLAGS) $(OPT) $(ARCH) $(CFLAGS) -c $< -o $@

$(EXEC7).exe: $(EXEC7).o
	@ echo Building $@...
	@ $(CC) -o $@ $< $(LDFLAGS) $(LDFLAGS)

$(EXEC8).o: $(EXEC8).c
	@ echo Compiling $<...
	@ $(CC) $(CCSTD) $(WFLAGS) $(OPT) $(ARCH) $(CFLAGS) -c $< -o $@

$(EXEC8).exe: $(EXEC8).o
	@ echo Building $@...
	@ $(CC) -o $@ $< $(LDFLAGS) $(LDFLAGS)

%.o : %.cpp Makefile
	@ echo Compiling $<...
	$(CXX) $(CXXSTD) $(WFLAGS) $(OPT) $(ARCH) $(CXXFLAGS) -c $< -o $@

# TODO: add targets for building executables

.PHONY: clean
clean:
	@ rm -f *.o *.exe
