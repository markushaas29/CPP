#include <iostream>
#include <cassert> 
#include <vector> 
#include <tuple> 
#include <memory> 
#include "../Matrix.hpp"
#include "../M3.hpp"
#include "../MatrixReader.hpp"
#include "../MatrixDescriptor.hpp"
#include "../MatrixProjector.hpp"
#include "../MatrixFilter.hpp"
#include "../../Common/DateTimes.hpp"
#include "../../CSV/Elements.hpp"
#include "../../CSV/Matcher.hpp"
#include "../../Quantity/Quantity.hpp"
#include "../../Functional/Functional.hpp"

class MatrixParserTest
{
	public:
		int Run()
		{
		    using MDS2 = MatrixDescriptor<2,std::string>;
		    using MDI2 = MatrixDescriptor<2,int>;
		    using MDD2 = MatrixDescriptor<2,double>;
		    using MDS1 = MatrixDescriptor<1,std::string>;
		    using MDD1 = MatrixDescriptor<1,double>;
		    using MS1 = Matrix<1,MDS1>;
		    using MD1 = Matrix<1,MDD1>;
		    using MD2 = Matrix<2,MDD2>;
		    using MS2 = Matrix<2,MDS2>;
		    using MI2 = Matrix<2,MDI2>;
			using ME2D = MatrixDescriptor<2,std::shared_ptr<IElement>>;
		    using M2E = Matrix<2,ME2D>;
		
			std::cout<<"START"<<std::endl;

			auto v = std::make_unique<std::vector<std::unique_ptr<IToken>>>();
			v->push_back(std::make_unique<DateToken>());
			v->push_back(std::make_unique<IBANToken>("DE19660623660009232702"));
			v->push_back(std::make_unique<BICToken>());
			v->push_back(std::make_unique<WordToken>());
			v->push_back(std::make_unique<SumToken>());
			v->push_back(std::make_unique<EntryToken>());
			v->push_back(std::make_unique<ValueToken>());
			v->push_back(std::make_unique<QuantityToken>());
			v->push_back(std::make_unique<KeyValueToken>());

			Matcher matcher(std::move(v));
		
			MS2 ms22{
				{std::string("1"),std::string("2")} ,
				{std::string("5"),std::string("6")} 
		    };

			M2E m2e {                
				{std::make_shared<Quantity<Sum>>(2.5), std::make_shared<Quantity<Sum>>(3.5)},
				{std::make_shared<Quantity<Sum>>(4.5), std::make_shared<Quantity<Sum>>(3.5)},
			};

			auto mq = ms22.Parse(matcher);
			
			std::cout<<"Quanti"<<mq<<std::endl;

			std::cout<<"END"<<std::endl;
		   
			return 0;
		}
};
