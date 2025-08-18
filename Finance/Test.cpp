#include <iostream>
#include <cassert>
#include "FinanceCategory.hpp"
#include "../Common/Color.hpp"
#include "../Unit/UnitRatio.hpp"
#include "../CSV/Element.hpp"
#include "../CSV/HtmlElement.hpp"
#include "../CSV/CssStyle.hpp"
#include "../CSV/Style.hpp"

class FinanceTest
{
	public:
int Run()
{       

    std::cout<<"START FinanceTest"<<std::endl;

	PaymentCategory f;
	std::cout<<f<<std::endl;
    
	std::cout<<"END"<<std::endl;

    return 0;
}
};
