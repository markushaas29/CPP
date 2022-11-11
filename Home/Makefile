lsCXX      = g++
CXXC      = clang
OBJECTS = *.o
CXXFLAGS  = -c -std=c++2a
DEBUGFLAGS  = -c -g -std=c++17

SRCDIR := src
OBJDIR := obj
BINDIR := bin

Release: $(OBJECTS)
	$(CXX) -o $@ $(OBJECTS) 
	
Debug: House.o Stage.o
	$(CXX) -o $@ $(OBJECTS) 

House: $(OBJECTS)
	$(CXX) -o $@ $(OBJECTS) 

Stage.o: Stagetest.cpp 
	$(CXX) $(DEBUGFLAGS) $<

House.o: HouseTest.cpp
	$(CXX) $(DEBUGFLAGS) $<
clean:
	rm *.o test




