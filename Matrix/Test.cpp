#include <iostream>
#include "Matrix.hpp"

int main()
{       
	std::cout<<"START"<<std::endl;
	
	using M3 = Matrix<3>;
	std::cout<<M3::LiteralType<<std::endl;

	std::cout<<"END"<<std::endl;

    return 0;
}
