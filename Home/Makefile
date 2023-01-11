CXX      = g++-11
CXXC      = clang
OBJECTS = Stagetest.o
CXXFLAGS  = -c -std=c++20
DEBUGFLAGS  = -c -g -std=c++20

SRCDIR := src
OBJDIR := obj
BINDIR := bin

Release:	$(OBJECTS)
	$(CXX) -o $@ $(OBJECTS) 
	
Debug:	$(OBJECTS)
	$(CXX) -o $@ $(OBJECTS) 

%.o: 	%.hpp %.cpp
	$(CXX) $(DEBUGFLAGS) $*.cpp
	
House: 	HouseTest.cpp
	$(CXX) -std=c++2a -o House HouseTest.cpp

Backup:	Backup.cpp
	g++ -std=c++2a -o Backup Backup.cpp

clean:
	rm -r  ../../../Downloads/* && rm *.o
	
All:
	$(MAKE) clean
	$(MAKE) Debug




