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
	
Counter: 	CounterTest.cpp
	$(CXX) -std=c++2a -o Counter CounterTest.cpp

Reading:	ReadingTest.cpp
	$(CXX) -std=c++2a -o Reading ReadingTest.cpp

AncillaryItems:	AncillaryItemsTest.cpp
	$(CXX) -std=c++2a -o AncillaryItems AncillaryItemsTest.cpp

clean:
	rm -r  ../../../Downloads/* && rm *.o
	
All:
	$(MAKE) clean
	$(MAKE) Debug




