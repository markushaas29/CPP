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

int main()
{
	FinanceTest sut;
	sut.Run();

	return 0;
}

