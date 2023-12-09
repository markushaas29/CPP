#include <iostream>
#include <cassert> 
#include <vector> 
#include <tuple> 
#include <memory> 
#include "Visitor.hpp"
#include "ElementVisitor.hpp"
#include "../CSV/Elements.hpp"
#include "../Common/Date.hpp"
#include "../Quantity/Quantity.hpp"
#include "../Matrix/Matrix.hpp"
#include "../Matrix/MatrixDescriptor.hpp"
#include "../Matrix/MatrixFilter.hpp"
#include "../Common/DateTimes.hpp"
#include "../CSV/Elements.hpp"
#include "../CSV/Matcher.hpp"

class VisitorTest{
	public:
int Run()
{
	std::cout<<"START Visitor"<<std::endl;

	ElementVisitor vd;
	std::shared_ptr<IElement> dp = std::make_shared<Date>(29,9,1986);
	std::shared_ptr<IElement> sp = std::make_shared<Quantity<Sum>>(29);
	auto qs = Quantity<Sum>(29);
	IElement* ie = &qs;
	dp->Accept(vd);
	sp->Accept(vd);
	ie->Accept(vd);

	const auto cd = Date(1,1,2020);
	cd.Accept(vd);

	auto pv = PredicateVisitor(std::make_unique<Quantity<Sum>>(29));
	assert(sp->Is(pv));
	assert(sp->Is(pv));
	auto pv228 = PredicateVisitor(std::make_unique<Quantity<Sum>>(228));
	assert(!sp->Is(pv228));
	auto pv29_9 = PredicateVisitor(std::make_unique<Date>(29,9,1986));
	assert(dp->Is(pv29_9));
	auto pv28_9 = PredicateVisitor(std::make_unique<Date>(28,9,1986));
	auto pv289 = PredicateVisitor(std::make_unique<Date>("28.9.1986"));
	assert(!dp->Is(pv28_9));

    using ME2D = MatrixDescriptor<2,std::shared_ptr<IElement>>;
    using M2E = Matrix<2,ME2D>;
	using MDS2 = MatrixDescriptor<2,std::string>;
    using MS2 = Matrix<2,MDS2>;

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
    M2E m2e {
        {std::make_shared<Quantity<Sum>>(2.5), std::make_shared<Quantity<Sum>>(3.5)},
        {std::make_shared<Quantity<Sum>>(4.5), std::make_shared<Quantity<Sum>>(3.5)},
    };
	MS2 ms22{
             {std::string("1,00"),std::string("2,00"),std::string("2.2.1989"), std::string("DE82660501011021592702")} ,
             {std::string("5,00"),std::string("6,00"),std::string("2.2.1989"), std::string("DE82660501011021592702")}
    };

    auto mq = ms22.Parse(matcher);
    MatrixVisitor<TransferVisitor> mv;
    mq.Accept(mv);

	TransferVisitor tv;
	auto d = Date(29,9,1986);
	tv.Visit(d);
	assert(tv.Is(pv29_9));
	assert(!tv.Is(pv28_9));
	assert(!tv.Is(pv228));

	std::cout<<"END Visitor"<<pv289<<std::endl;
	std::cout<<"END Visitor"<<std::endl;
   
	return 0;
}
};
