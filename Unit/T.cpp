#include <iostream>
#include <cassert> 
#include <vector> 
#include <tuple> 
#include <memory> 
#include "UnitTest.cpp"
#include "../CSV/HtmlElement.hpp"
#include "../CSV/Styles.hpp"
#include "../CSV/CssStyle.hpp"   
#include "../CSV/Style.hpp"
#include "../Common/Color.hpp"
#include "../Quantity/QuantityRatioTest.cpp"

int main()
{
	UnitTest u;
	QRTest sut2;

	u.Run();
	sut2.Run();

	return 0;
}

