CXX      = g++-11
CXXC      = clang
OBJECTS = *.o
CXXFLAGS  = -c -std=c++2a
DEBUGFLAGS  = -c -g -std=c++2a

SRCDIR := src
OBJDIR := obj
BINDIR := bin

Release:	$(OBJECTS)
	$(CXX) -o $@ $(OBJECTS) 
	
Debug:	$(OBJECTS)
	$(CXX) -o $@ $(OBJECTS) 

%.o: 	%.hpp %.cpp
	$(CXX) $(DEBUGFLAGS) $*.cpp
	
Regex: 	RegexTest.cpp
	g++ -std=c++2a -o Regex RegexTest.cpp

Cont:	TransferContainerTest.cpp
	g++-11 -std=c++2a -o Cont TransferContainerTest.cpp

Item:	TransferItemContainerTest.cpp
	g++-11 -std=c++2a -o Item TransferItemContainerTest.cpp
	
clean:
	rm -r  ../../../Downloads/* && rm *.o
	
All:
	$(MAKE) clean
	$(MAKE) Debug




