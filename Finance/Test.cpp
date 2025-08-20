#include <iostream>
#include <cassert>
#include "PaymentCategory.hpp"
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

	PaymentCategory f{Quantity<Scalar>{0.5}};
	std::cout<<f<<std::endl;
    assert(f.Current()==Quantity<Sum>{100});
    assert(f.Available()==Quantity<Sum>{0});
    assert(f.Proportion()==Quantity<Scalar>{0.5});
    
	f(Quantity<Sum>{1000});//==Quantity<Sum>{100});
    assert(f.Available()==Quantity<Sum>{500});

	std::cout<<"END"<<std::endl;

    return 0;
}
};
