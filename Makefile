CXX      = g++-11
CXXC      = clang
OBJECTS = *.o
CXXFLAGS  = -c -std=c++23
DEBUGFLAGS  = -c -g -std=c++17

SRCDIR := src
OBJDIR := obj
BINDIR := bin

all:
	+$(MAKE) -C File
	+$(MAKE) -C Home
	
Debug: $(OBJECTS)
	$(CXX) -o $@ $(OBJECTS) 

%.o: %.cpp %.h %.hpp
	$(CXX) $(DEBUGFLAGS) $<

clean:
	rm *.o test

