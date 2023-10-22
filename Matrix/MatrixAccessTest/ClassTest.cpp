#include <iostream>
#include <cassert> 
#include <vector> 
#include <tuple> 
#include "../Matrix.hpp"
#include "../MatrixDescriptor.hpp"

class Base 
{
public:
	friend std::ostream& operator<<(std::ostream& s, const Base& m) { return s<<"Base: "<<m.value; }
	Base(int v): value{v} {}
	virtual ~Base() {}
private:
	int value;
};

class BSub :public Base
{
public:
	BSub(int v): Base{v} {}
	virtual std::ostream& operator<<(std::ostream& s) const { return s<<"B"; }
	friend std::ostream& operator<<(std::ostream& s, const BSub& m) { return s<<"BSub: "; }
};


class CSub :public Base
{
public:
	CSub(int v): Base{v} {}
	virtual std::ostream& operator<<(std::ostream& s) const { return s<<"C"; }
	friend std::ostream& operator<<(std::ostream& s, const CSub& m) { return s<<"CSub: "; }
};

int main()
{
		    using MDS2 = MatrixDescriptor<2,std::string>;
		    using MDI2 = MatrixDescriptor<2,int>;
		    using MDD2 = MatrixDescriptor<2,double>;
		    using MDS1 = MatrixDescriptor<1,std::string>;
		    using MDD1 = MatrixDescriptor<1,double>;
		    using MDA1 = MatrixDescriptor<1,std::shared_ptr<Base>>;
		    using MS1 = Matrix<1,MDS1>;
		    using MA1 = Matrix<1,MDA1>;
		    using MD1 = Matrix<1,MDD1>;
		    using MD2 = Matrix<2,MDD2>;
		    using MS2 = Matrix<2,MDS2>;
		    using MI2 = Matrix<2,MDI2>;
		
			std::cout<<"START Class"<<std::endl;
		
			MS1 ms1{
				{std::string("1"),std::string("2")} 
		    };
		
			std::array<size_t,1> a1{2};
			MDA1 md1(a1);
			auto v = std::vector<std::shared_ptr<Base>> {std::make_shared<BSub>(2), std::make_shared<CSub>(3)};

			MA1 ma1(md1, v);
			std::cout<<"M Base"<<ma1<<std::endl;

			auto b = ma1[0];
			std::cout<<"M Base"<<b.As<BSub>()<<std::endl;
			auto bp = b.Value();
			auto bpp =	std::dynamic_pointer_cast<BSub>(bp);
			std::cout<<"M Base"<<*bpp<<std::endl;
		
		 	MI2 m35 {
		        {1, 2, 3, 4, 5},
		        {6, 7, 8, 9,10},
		        {11, 12, 13, 14, 15},
		    };
		 	
			MI2 m33 {
		        {3, 4, 5},
		        {8, 9,10},
		        {13, 14, 15},
		    };
			
			MD2 m33D {
		        {3, 4, 5},
		        {8, 9,10},
		        {13, 14, 15},
		    };

			std::cout<<"END"<<m35.M({1,2},{2,2})<<std::endl;
		   
			return 0;
}
