#include <iostream>
#include "Matrix.hpp"

int main()
{       
	std::cout<<"START"<<std::endl;
	
	using M3 = Matrix<3>;
	using M2 = Matrix<2>;
	std::cout<<M3::LiteralType<<std::endl;

	M2 m2 {
		{1,2},
		{3,4},
	};

	std::cout<<"END"<<std::endl;

    return 0;
}
