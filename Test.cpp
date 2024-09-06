#include <iostream>
#include <chrono>
#include <vector>
#include <sstream>
#include "Logger/Logger.hpp"
#include "Quantity/ToQuantity.hpp"
#include "Common/DateTimesTest.cpp"
#include "Quantity/Test.cpp"
#include "Matrix/Test.cpp"
#include "String/Test.cpp"
#include "Visitor/Test.cpp"
#include "Functional/Test.cpp"
#include "Expression/Test.cpp"
#include "ObjectFactory/Test.cpp"
#include "Singleton/Test.cpp"
#include "Counter/Test.cpp"
//#include "Decorator/Test.cpp"
#include "Repository/Test.cpp"
#include "HTMLBuilder/Test.cpp"
#include "CSV/ElementTest/ElementTest.cpp"


int main()
{   
	std::cout<<"START"<<std::endl;

	String_Test st;
	st.Run();
	
	DateTest dt;
	dt.Run();
	
	VisitorTest vt;
	vt.Run();
	
	QTest qt;
	qt.Run();
	
	MatrixTest mt;
	mt.Run();
	
	FactoryTest ft;
	ft.Run();
	
	FunctionalTest fut;
	fut.Run();

	ElementTest et;
	et.Run();
	
	SingletonTest s;
	s.Run();
	
	RepositoryTest r;
	r.Run();
	
	ExpressionTest ex;
	ex.Run();
	
//	DecoratorTest dec;
//	dec.Run();
	
	HTMLBuilderTest html;
	html.Run();
	
	CounterTest c;
	c.Run();
	std::cout<<"END"<<std::endl;
	return 0;
}
