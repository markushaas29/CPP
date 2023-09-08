CXX      = g++-11
CXXC      = clang
OBJECTS = *.o
CXXFLAGS  = -c -std=c++23
NFLAGS  = -c -D NDEBUG -std=c++23
DEBUGFLAGS  = -c -g -std=c++17

SRCDIR := src
OBJDIR := obj
BINDIR := bin

Release: $(OBJECTS)
	$(CXX) -o $@ $(OBJECTS) 
	
Debug: $(OBJECTS)
	$(CXX) -o $@ $(OBJECTS) 

NDebug: $(OBJECTS)
	$(CXX) -o $@ $(OBJECTS) 

%.o: %.cpp %.h %.hpp
	$(CXX) $(NFLAGS) $<

T: 	T.cpp
	g++ -std=c++2a -o T T.cpp

clean:
	rm *.o test

