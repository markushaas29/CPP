CXX      = g++-11
CXXC      = clang
OBJECTS = Test.o
ROBJECTS = QuantityRatioTest.o
CXXFLAGS  = -c -std=c++2a
DEBUGFLAGS  = -c -g -std=c++17

SRCDIR := src
OBJDIR := obj
BINDIR := bin

Ratio: $(ROBJECTS)
	$(CXX) -o $@ $(ROBJECTS) 
	
Debug: $(OBJECTS)
	$(CXX) -o $@ $(OBJECTS) 

%.o: %.cpp %.h
	$(CXX) $(DEBUGFLAGS) $<
	
Regex: 	RegexTest.cpp
	g++ -std=c++2a -o Regex RegexTest.cpp

clean:
	rm *.o test




