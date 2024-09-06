#include <iostream>
#include <cassert> 
#include <vector> 
#include <tuple> 
#include <memory> 
#include "Test.cpp"
#include "../CSV/HtmlElement.hpp"
#include "../CSV/Styles.hpp"
#include "../CSV/CssStyle.hpp"   
#include "../CSV/Style.hpp"
#include "../Common/Color.hpp"
#include "QuantityRatioTest.cpp"

int main()
{
	QTest sut;
	QRTest sut2;

	sut.Run();
	sut2.Run();

	return 0;
}

