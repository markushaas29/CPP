#include <iostream>
#include <typeinfo>
#include <typeindex>
#include "Is.hpp"
#include "../String/Literal.hpp"

template<Literal L>
struct I
{
	I() {  }
};

int main()
{       
    std::cout<<"START"<<std::endl;
	I<"Test"> i();	
	Is<CompileTime,3==3>()(2 == 3);
	//Is<CompileTime,2==3>()(2 == 3);
	
	auto s = "2==3";
	auto il = Is<Logging>(s);
	il(2 == 3);
	il(3 == 3);
	auto it = Is<Throwing>(s);
	//it(2 == 3);
	it(3 == 3);
	auto itt = Is<Terminating>(s);
	itt(3==3);
	//Is<int>()(2 == 3);
	std::cout<<"END"<<std::endl;

	//assert(2>3);	
    return 0;
}
