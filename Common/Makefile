CXX      = g++-11
CXXC      = clang
OBJECTS = *.o
CXXFLAGS  = -c -std=c++23
DEBUGFLAGS  = -c -g -std=c++17

SRCDIR := src
OBJDIR := obj
BINDIR := bin

Release: $(OBJECTS)
	$(CXX) -o $@ $(OBJECTS) 
	
Debug: T.cpp
	g++-11 -c -g -std=c++2a -o *.o T.cpp 

%.o: %.cpp %.h %.hpp
	$(CXX) $(DEBUGFLAGS) $<

clean:
	rm *.o test

