CXX      = g++
CXXC      = clang
OBJECTS = Test.o Wrapper.o
CXXFLAGS  = -c -std=c++2a
DEBUGFLAGS  = -c -g -std=c++2a

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




