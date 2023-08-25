#include <iostream>
#include <vector>
#include "To.hpp"
#include "../../Common/DateTimes.hpp"
#include "../../Quantity/Quantity.hpp"
#include "../../Logger/Logger.hpp"
#include "../../Common/ParseResult.hpp"
using namespace String_;
using namespace DateTimes;

int main()
{       
	std::cout<<"1.2_"<<ParseTo("1.2")<<std::endl;
	std::cout<<"1.2_"<<ParseTo<double>("1.2")<<std::endl;
	double d = TryTo<double>("1.2");
	std::string arg = "1,3";
	std::replace( arg.begin(), arg.end(), Comma::Value, Point::Value);
	std::cout<<"arg_"<<arg<<" "<<std::endl;
	
	std::cout<<"1,3_"<<ParseTo<double>("1,3")<<" "<<std::endl;
	std::cout<<"1,4_"<<ParseTo<double>("1,4")<<" "<<std::endl;

	assert(TryTo<double>("1.123"));
	assert((double)TryTo<double>("1.123")==1.123);
	assert((double)ParseTo<double>("1,123")==1.123);
	assert(TryTo<uint>("123"));
	assert(!TryTo<uint>("s"));
	assert(!TryTo<uint>(""));

	std::string ds1{"24 12 2022"};
	assert(TryTo<Date>(ds1));

	std::string ds2{"32 12 2022"};
	auto d2 = TryTo<Date>(ds2);
	
	std::cout<<d2<<" "<<std::endl;
	assert(!d2);

	using QL = Quantity<Length,Kilo>;
	std::string dq1{"32"};
	auto ql = TryTo<QL>(ds2);
	std::cout<<ql<<std::endl;
    
	return 0;
}
