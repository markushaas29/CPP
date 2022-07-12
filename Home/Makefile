lsCXX      = g++
CXXC      = clang
OBJECTS = *.o
CXXFLAGS  = -c -Wdeprecated-enum-enum-conversion -std=c++2a
DEBUGFLAGS  = -c -g -std=c++17

SRCDIR := src
OBJDIR := obj
BINDIR := bin

Release: $(OBJECTS)
	$(CXX) -o $@ $(OBJECTS) 
	
Debug: $(OBJECTS)
	$(CXX) -o $@ $(OBJECTS) 

%.o: %.cpp %.h
	$(CXX) $(DEBUGFLAGS) $<

clean:
	rm *.o test




