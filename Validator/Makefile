CXX      = g++
CXXC      = clang
OBJECTS = *.o
CXXFLAGS  = -c -D NDEBUG -std=c++2a
DEBUGFLAGS  = -c -std=c++17 -NDEBUG

SRCDIR := src
OBJDIR := obj
BINDIR := bin

Release: $(OBJECTS)
	$(CXX) -o $@ $(OBJECTS) 
	
Debug: $(OBJECTS)
	$(CXX) -o $@ $(OBJECTS) 

%.o: 	%.hpp %.cpp %.h
	$(CXX) $(DEBUGFLAGS) $<

clean:
	rm *.o test




