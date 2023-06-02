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

	M2 m21 {
		{1,2,1,2},
		{3,4,3,4},
		{3,4,3,4},
	};
	M2 m22 {
		{1,2,1,2,5},
		{3,4,3,4,5},
		{3,4,3,4,5},
	};
	M3 m3 {
		{{1,2},{3,4}},
		{{5,6},{7,8}},
		{{9,10},{11,12}}
	};

	m3.Row(1);

	std::cout<<"END"<<std::endl;

    return 0;
}
