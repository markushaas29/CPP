#include <iostream>
#include <cassert> 
#include <vector> 
#include <tuple> 
#include <memory> 
#include "../PointerConcept.hpp"
#include "../MatrixElement.hpp"
#include "../MatrixIdentifier.hpp"
#include "../../Quantity/Quantity.hpp"
#include "../../CSV/Elements.hpp"
#include "../../Common/DateTimes.hpp"

using QS = Quantity<Sum>; 
using namespace DateTimes; 
class MatrixElementTest
{
	public:
int Run()
{
	std::cout<<"START"<<std::endl;

	std::vector<std::string> s{"1","2","27.12.2022"};
	std::vector<std::shared_ptr<std::string>> ss{std::make_shared<std::string>("9")};
	
	if constexpr (PointerConcept<decltype(ss[0])>)
	{
		assert(true);
	}
	else
	{
		assert(false);
	}
	auto pi = std::make_shared<int>(9);
	if constexpr (PointerConcept<decltype(pi)>)
	{
		assert(true);
	}
	else
	{
		assert(false);
	}

	auto i = int{9};
	if constexpr (PointerConcept<decltype(i)>)
	{
		assert(false);
	}
	else
	{
		assert(true);
	}
	
	auto sT = std::string("T");
	if constexpr (PointerConcept<decltype(sT)>)
	{
		assert(false);
	}
	else
	{
		assert(true);
	}

//	auto mI = Multiplication::Calculate(1,2) *  Multiplication::Calculate(3,3)*  Multiplication::Calculate(3,3);
//	std::cout<<mI<<std::endl;
//
//	auto mi = MatrixElement<decltype(mI)>(mI);
//	std:: cout<<mi<<"\n ";//<<mi.Get()<<std::endl;
//	assert((uint)mi==162);
//	assert((double)mi==162.0);
//	assert(mi.To<uint>()==162);
//	assert(mi.To<double>()==162);
//	assert(mi()==162);

	auto md = MatrixElement<double>(5.0);
	std:: cout<<md<<"\n ";//<<mi.Get()<<std::endl;
	assert((double)md==5.0);
	assert(md.To<uint>()==5);
	assert(md()==5.0);
	
	auto mu = MatrixElement<uint>(5);
	std:: cout<<mu<<"\n ";//<<mi.Get()<<std::endl;
	assert((int)mu==5);
	assert(mu.To<double>()==5.0);
	assert(mu()==5);
	
	auto ms = MatrixElement<std::string>("5");
	std:: cout<<ms<<"\n ";//<<mi.Get()<<std::endl;
	assert((std::string)ms=="5");
	assert(ms()=="5");

	assert(ms.To<uint>()==5);
	assert(ms.To<double>()==5.0);
	assert(ms.To<int>()==5);

	auto m123 = MatrixIdentifier(std::string("1_2_3"));
	auto m123_ = m123;
	assert(m123==std::string("1_2_3"));
	assert(m123!=std::string("4"));
	assert(m123==std::string("3"));
	assert(m123==std::string("2"));
	assert(m123==std::string("1"));
	assert(m123.Size()==3);

	auto miban = MatrixIdentifier(std::string("1_2_3_DE19660623660009232702"));
	assert(miban==IBAN("DE19660623660009232702"));
	assert(miban!=IBAN("DE19460623660009232702"));
	assert(miban!=Entry("123"));
	assert(miban.Size()==4);
	std::cout<<"ID"<<m123_<<std::endl;
	
	auto m12 = MatrixIdentifier(std::string("12"));
	auto m12T = m12.To<int>();
	assert(m12T==12);
	
	auto m789 = MatrixIdentifier<int>(789);
	assert(m789==789);
	assert(m789!=779);

	std::cout<<"END"<<std::endl;
    
	return 0;
}	
};
