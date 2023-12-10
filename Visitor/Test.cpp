#include <iostream>
#include <cassert> 
#include <vector> 
#include <tuple> 
#include <memory> 
#include "Visitor.hpp"
#include "ElementVisitor.hpp"
#include "PredicateVisitor.hpp"
#include "../CSV/Elements.hpp"
#include "../ObjectFactory/Factory.hpp"
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
	std::shared_ptr<IElement> ip = std::make_shared<IBAN>(std::string("DE82660501011021592702"));
	auto qs = Quantity<Sum>(29);
	IElement* ie = &qs;
	dp->Accept(vd);
	sp->Accept(vd);
	ie->Accept(vd);

	const auto cd = Date(1,1,2020);
	cd.Accept(vd);

	auto pv = EqualVisitor(std::make_shared<Quantity<Sum>>(29));
	assert(sp->Is(pv));
	assert(sp->Is(pv));
	auto pv228 = EqualVisitor(std::make_shared<Quantity<Sum>>(228));
	assert(!sp->Is(pv228));
	auto qlv = LessVisitor(std::make_shared<Quantity<Sum>>(29));
	assert(!sp->Is(qlv));
	auto qlv2 = LessVisitor(std::make_shared<Quantity<Sum>>(28));
	assert(!sp->Is(qlv2));

	auto pv29_9 = EqualVisitor(std::make_shared<Date>(29,9,1986));
	assert(dp->Is(pv29_9));
	auto pv28_9 = EqualVisitor(std::make_shared<Date>(28,9,1986));
	auto pv289 = EqualVisitor(std::make_shared<Date>("28.9.1986"));
	assert(!dp->Is(pv28_9));
	auto dlv289 = LessVisitor(std::make_shared<Date>("30.9.1986"));
	assert(dp->Is(dlv289));
	auto dlv299 = LessVisitor(std::make_shared<Date>("29.9.1986"));
	assert(!dp->Is(dlv299));

	auto iv1 = EqualVisitor(std::make_shared<IBAN>(std::string("DE82660501011021592702")));
	assert(ip->Is(iv1));
	auto iv2 = EqualVisitor(std::make_shared<IBAN>(std::string("DE82660501011021592703")));
	assert(!ip->Is(iv2));
	auto ilv1 = LessVisitor(std::make_shared<IBAN>(std::string("DE82660501011021592702")));
	assert(!ip->Is(ilv1));
	auto ilv2 = LessVisitor(std::make_shared<IBAN>(std::string("DE82660501011021590002")));
	assert(!ip->Is(ilv1));

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
	assert(!tv.Is(dlv299));
	assert(tv.Is(dlv289));
	
	Factory<IElement> fme;
    fme.Register("Q",[](const std::string s) { return std::make_unique<IBAN>(s); });
    assert(fme.Size()==1);

	Factory<IPredicateVisitor, std::shared_ptr<IElement>> fm;
    fm.Register("EQ",[](std::shared_ptr<IElement> p) { return std::make_unique<EqualVisitor>(std::move(p)); });
    assert(fm.Size()==1);

	std::cout<<"END Visitor"<<pv289<<std::endl;
	std::cout<<"END Visitor"<<std::endl;
   
	return 0;
}
};
