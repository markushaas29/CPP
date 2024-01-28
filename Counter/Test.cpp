#include <iostream>
#include <cassert> 
#include <vector> 
#include <memory> 
#include "CounterHeaders.hpp"
#include "../ObjectFactory/Factory.hpp"
#include "../Common/DateTimes.hpp"
#include "../CSV/Elements.hpp"
#include "../CSV/Matcher.hpp"

class CounterTest
{
	public:
		int Run()
		{
			std::cout<<"START COunterTest"<<std::endl;
			auto tf = Build<IToken, WorkToken, VolumeToken, WordToken, SumToken, IBANToken, DateToken, BICToken, EmptyToken, IDToken, ValueToken, QuantityToken, WordToken>();
			Counter<int> c{tf};

			std::cout<<"Counter\n"<<c<<std::endl;

			std::cout<<"END"<<std::endl;
		   
			return 0;
		}
};
