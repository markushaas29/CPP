CXX      = g++
CXXC      = clang
OBJECTS = UnitTest.o
CXXFLAGS  = -c -std=c++2a
DEBUGFLAGS  = -c -g -std=c++17

SRCDIR := src
OBJDIR := obj
BINDIR := bin

Release: $(OBJECTS)
	$(CXX) -o $@ $(OBJECTS) 
	
Debug: $(OBJECTS)
	$(CXX) -o $@ $(OBJECTS) 

%.o: %.cpp %.h %.hpp
	$(CXX) $(DEBUGFLAGS) $<

clean:
	rm *.o test

