#include <iostream>
#include <cassert> 
#include <vector> 
#include <tuple> 
#include <memory> 
#include "Registration.hpp"
#include "../Common/DateTimes.hpp"
#include "../CSV/Elements.hpp"
#include "../Visitor/PredicateVisitor.hpp"
#include "../CSV/Token.hpp"
#include "../Quantity/Quantity.hpp"
#include "../ObjectFactory/Factory.hpp"

class TypeRegistrationTest
{
	public:
		int Run()
		{
			std::cout<<"START TypeRegistration"<<std::endl;


			auto fmt=std::make_shared<Factory<IElement>>();
			auto reg = Registration<Factory<IElement>,Quantity<Sum>, IBAN, Date, BIC, ID<std::string>, Name, Index, Empty>(&(*fmt));

			auto qp = (*fmt)("Sum","100");
			std::cout<<fmt<<std::endl;
			
			Factory<IToken> fmt2;
			auto reg2 = Registration<Factory<IToken>,SumToken, IBANToken, DateToken, BICToken, EmptyToken, IDToken, WordToken>(&fmt2);
			auto qp2 = fmt2("SumToken","100");
			std::cout<<*qp2<<std::endl;

			auto pfs = std::make_shared<CompositeFactory<IPredicateVisitor, Factory<IElement>>>(fmt);
     		pfs->Register("EQ",[](std::unique_ptr<IElement> e) { return std::make_unique<EqualVisitor>(std::move(e)); });
			auto regC = Registration<CompositeFactory<IPredicateVisitor, Factory<IElement>>,EqualVisitor>(&(*pfs));

			auto tf = TypeFactory<Factory<IElement>, Quantity<Sum>, IBAN, Date, BIC, ID<std::string>, Name, Index, Empty>();
			auto tfc = TypeFactory<CompositeFactory<IPredicateVisitor, Factory<IElement>>, EqualVisitor, LessVisitor>(fmt);

			auto eqv = tfc("EqualVisitor", { "IBAN", "DE82660501011021592702"});
			auto eqvq = tfc("EqualVisitor", { "Sum", "29"});
			auto eqvd = tfc("EqualVisitor", { "Date", "29.9.1986"});
			auto lvq = tfc("LessVisitor", { "Sum", "30"});
			auto ib = IBAN(std::string("DE82660501011021592702"));
			std::unique_ptr<IElement> dp = std::make_unique<Date>(29,9,1986);
			std::unique_ptr<IElement> d20112022 = std::make_unique<Date>("20.11.2022");
			std::unique_ptr<IElement> sp = std::make_unique<Quantity<Sum>>(29);
			std::unique_ptr<IElement> ip = std::make_unique<IBAN>(std::string("DE82660501011021592702"));
			//assert(eqv->Visit(ib));
			assert(ip->Is(*eqv));
			assert(dp->Is(*eqvd));
			assert(sp->Is(*lvq));

			//auto tfc = TypeFactory<CompositeFactory<IPredicateVisitor, Factory<IElement>>, EqualVisitor, LessVisitor, AndVisitor>(fmt);
	//		auto regT = Registration<TypeFactory<IToken>>(&tf);//,SumToken, IBANToken, DateToken, BICToken, EmptyToken, IDToken, WordToken>(&fmt2);

			std::cout<<"END TypeRegistration"<<std::endl;

			return 0;
		}
};

