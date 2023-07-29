#include <iostream>
#include <cassert> 
#include <vector> 
#include <tuple> 
#include <memory> 
#include "../MatrixElement.hpp"
#include "../MatrixParser.hpp"
#include "../../Quantity/Quantity.hpp"
#include "../../CSV/Elements.hpp"
#include "../../Common/DateTimes.hpp"
#include "../../Calculator/Operations.hpp"
using QS = Quantity<Sum>; 
using namespace DateTimes; 

int main()
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

	auto mI = Multiplication::Calculate(1,2) *  Multiplication::Calculate(3,3)*  Multiplication::Calculate(3,3);
	std::cout<<mI<<std::endl;

	auto mi = MatrixElement<decltype(mI)>(mI);
	std:: cout<<mi<<"\n ";//<<mi.Get()<<std::endl;
	assert((uint)mi==162);

	auto md = MatrixElement<double>(5.0);
	std:: cout<<md<<"\n ";//<<mi.Get()<<std::endl;
	assert((double)md==5.0);
	
	auto mu = MatrixElement<uint>(5);
	std:: cout<<mu<<"\n ";//<<mi.Get()<<std::endl;
	assert((int)mu==5);
	
	auto ms = MatrixElement<std::string>("5");
	std:: cout<<ms<<"\n ";//<<mi.Get()<<std::endl;
	assert((std::string)ms=="5");

//	auto vei = ValueElement<IBAN>("IBANT");
//	auto i = vei.Get();
//	bool same = std::is_same_v<decltype(i),IBAN>;
//	assert(same);
//
//	auto qe = QuantityElement<QS>("1.53");
//	std::cout<<qe<<std::endl;
//	auto q = qe.Get();
//	same = std::is_same_v<decltype(q),QS>;
//	assert(same);
//	
//	std::unique_ptr<Element<QS>> ptr = std::make_unique<QS>("");
//
//	auto vd = ValueElement<DateTimes::Date>("1.1.2022");
//	auto dp = vd.Ptr();
//	auto d = vd.Get();
//	std::cout<<d<<std::endl;
//	same = std::is_same_v<decltype(d),DateTimes::Date>;
//	assert(same);
//
//	auto c = vd.Cast();
//
//	auto m3 = MatrixParser<std::tuple<Quantity<Sum>,IBAN,DateTimes::Date>>();
//	auto m4 = MatrixParser<std::tuple<Quantity<Sum>,IBAN,IBAN,DateTimes::Date>>();
//	bool a=false;
//	try { m4.Parse(s); }  catch(...)  {  a=true; }
//	assert(a);
//
//	auto t = m3.Parse(s);
//	std::cout<<std::get<0>(t)<<std::endl;
//	auto st = std::get<0>(t);
//
//	auto m1 = MatrixParser<std::string>();
//	auto t1 = m1.Parse(s);
//	assert(t1[0].Get()=="1");
//	assert(t1[2].Get()=="27.12.2022");
//	auto t2 = m1.Parse(ss);
//	assert(t2[0].Get()=="9");
//	
//	auto ms = MatrixParser<Quantity<Sum>>();
//	auto s1 = ms.Parse(s);
//	assert(s1[0]==1);
//	assert(s1[1]==2);
//	std::cout<<"Sum: "<<s1[2]<<std::endl;
//	
//	s1 = ms.Parse(ss);
//	assert(s1[0]==9);
//	std::cout<<"Sum 2: "<<s1[0]<<std::endl;
//	
//	std::cout<<std::get<0>(t)<<std::endl;
//	std::cout<<st.Get()<<std::endl;
//	std::cout<<std::get<2>(t)<<std::endl;
	std::cout<<"END"<<std::endl;
//
    return 0;
}
