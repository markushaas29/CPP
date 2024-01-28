#include <iostream>
#include <cassert> 
#include <vector> 
#include <memory> 
#include "CounterHeaders.hpp"
#include "../ObjectFactory/Factory.hpp"
#include "../Common/DateTimes.hpp"
#include "../Unit/Unit.hpp"
#include "../CSV/Elements.hpp"
#include "../CSV/Matcher.hpp"
#include "../Visitor/CollectorVisitor.hpp"

class CounterTest
{
	public:
		int Run()
		{
			std::cout<<"START COunterTest"<<std::endl;
			auto tf = Build<IToken, WorkToken, VolumeToken, WordToken, SumToken, IBANToken, DateToken, BICToken, EmptyToken, IDToken, ValueToken, QuantityToken, WordToken>();
			auto c = std::make_unique<Counter<Volume>>(tf);

			std::cout<<"Counter\n"<<*c<<std::endl;
			std::unique_ptr<BaseVisitor> civ = std::make_unique<ConsumptionVisitor<Quantity<Volume, Pure, double>>>();
			civ = c->Accept(std::move(civ));
            auto consV = civ->template As<ConsumptionVisitor<Quantity<Volume>>>();
			std::cout<<"Counter\n"<<*(consV())<<std::endl;

			std::cout<<"END"<<std::endl;
		   
			return 0;
		}
};
