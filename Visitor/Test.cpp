#include <iostream>
#include <cassert> 
#include <vector> 
#include <tuple> 
#include <memory> 
#include "Visitor.hpp"
#include "ElementVisitor.hpp"
#include "PredicateVisitor.hpp"
#include "CollectorVisitor.hpp"
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
	std::unique_ptr<IElement> dp = std::make_unique<Date>(29,9,1986);
	std::unique_ptr<IElement> d20112022 = std::make_unique<Date>("20.11.2022");
	std::unique_ptr<IElement> sp = std::make_unique<QS>(29);
	std::unique_ptr<IElement> ip = std::make_unique<IBAN>(std::string("DE82660501011021592702"));
	std::unique_ptr<IElement> ep = std::make_unique<Entry>(std::string("A B C D"));
	auto qs = QS(29);
	IElement* ie = &qs;
	dp->Accept(vd);
	sp->Accept(vd);
	ie->Accept(vd);

	const auto cd = Date(1,1,2020);
	cd.Accept(vd);

	auto ev = EqualVisitor(std::make_unique<Entry>(std::string("A")));
	assert(ep->Is(ev));
	ev = EqualVisitor(std::make_unique<Entry>(std::string("B")));
	assert(ep->Is(ev));
	ev = EqualVisitor(std::make_unique<Entry>(std::string("E")));
	assert(!ep->Is(ev));
	
	auto pv = EqualVisitor(std::make_unique<QS>(29));
	assert(sp->Is(pv));
	assert(sp->Is(pv));
	auto pv228 = EqualVisitor(std::make_unique<QS>(228));
	assert(!sp->Is(pv228));
	auto qlv = LessVisitor(std::make_unique<QS>(29));
	assert(!sp->Is(qlv));
	auto qlv2 = LessVisitor(std::make_unique<QS>(28));
	assert(!sp->Is(qlv2));

	auto pv29_9 = EqualVisitor(std::make_unique<Date>(29,9,1986));
	assert(dp->Is(pv29_9));
	auto pv28_9 = EqualVisitor(std::make_unique<Date>(28,9,1986));
	auto pv289 = EqualVisitor(std::make_unique<Date>("28.9.1986"));
	assert(!dp->Is(pv28_9));
	auto pv20112022 = EqualVisitor(std::make_unique<Date>(20,11,2022));
	assert(d20112022->Is(pv20112022));
	auto dlv289 = LessVisitor(std::make_unique<Date>("30.9.1986"));
	assert(dp->Is(dlv289));
	auto dlv299 = LessVisitor(std::make_unique<Date>("29.9.1986"));
	assert(!dp->Is(dlv299));

	auto iv1 = EqualVisitor(std::make_unique<IBAN>(std::string("DE82660501011021592702")));
	assert(ip->Is(iv1));
	auto iv2 = EqualVisitor(std::make_unique<IBAN>(std::string("DE82660501011021592703")));
	assert(!ip->Is(iv2));
	auto ilv1 = LessVisitor(std::make_unique<IBAN>(std::string("DE82660501011021592702")));
	assert(!ip->Is(ilv1));
	auto ilv2 = LessVisitor(std::make_unique<IBAN>(std::string("DE82660501011021590002")));
	assert(!ip->Is(ilv1));

//	auto sv = AccumulationVisitor<IBAN>();
//	auto ib = IBAN(std::string("DE82660501011021592702"));
//	ib.Accept(sv);
//	assert(sv.Size()==1);
//	
//	auto svd = AccumulationVisitor<Date>();
	auto d = Date(29,9,1986);
//	//svd.Visit(ib);
//	svd.Visit(d);
//	std::cout<<svd<<std::endl;
//	assert(svd.Size()==1);
    
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
        {std::make_shared<QS>(2.5), std::make_shared<QS>(3.5)},
        {std::make_shared<QS>(4.5), std::make_shared<QS>(3.5)},
    };
	MS2 ms22{
             {std::string("1,00"),std::string("2,00"),std::string("2.2.1989"), std::string("DE82660501011021592702"), std::string("A B E")} ,
             {std::string("5,00"),std::string("6,00"),std::string("2.2.1989"), std::string("DE82660501011021592702"), std::string("U V W")}
    };

    auto mq = ms22.Parse(matcher);
	TransferVisitor tv;
	tv.Visit(d);
	assert(tv.Is(pv29_9));
	assert(!tv.Is(pv28_9));
	assert(!tv.Is(pv228));
	assert(tv.Is(dlv299));
	assert(tv.Is(dlv289));
	
	auto e = Entry("A B E");
	tv.Visit(e);
	assert(tv.Is(ev));
	ev = EqualVisitor(std::make_unique<Entry>(std::string("B")));
	assert(tv.Is(ev));
	ev = EqualVisitor(std::make_unique<Entry>(std::string("C")));
	assert(!tv.Is(ev));
	
	auto fme =std::make_shared<Factory<IElement>>();
    fme->Register("I",[](const std::string s) { return std::make_unique<IBAN>(s); });
    assert(fme->Size()==1);
    fme->Register("D",[](const std::string s) { return std::make_unique<Date>(s); });
    assert(fme->Size()==2);
    fme->Register("Q",[](const std::string s) { return std::make_unique<Quantity<Sum,Pure,double>>(s); });
    assert(fme->Size()==3);
	
	auto pfs = std::make_shared<CompositeFactory<IPredicateVisitor, Factory<IElement>>>(fme);
	pfs->Register("EQ",[](std::unique_ptr<IElement> e) { return std::make_unique<EqualVisitor>(std::move(e)); });
    assert(pfs->Size()==1);
	pfs->Register("L",[](std::unique_ptr<IElement> e) { return std::make_unique<LessVisitor>(std::move(e)); });
    assert(pfs->Size()==2);
	auto eqv = ((*pfs)("EQ", { "I", "DE82660501011021592702"}));
	auto eqvq = ((*pfs)("EQ", { "Q", "29"}));
	auto eqvd = ((*pfs)("EQ", { "D", "29.9.1986"}));
	auto lvq = ((*pfs)("L", { "Q", "30"}));
    //assert(eqv->Visit(ib));
	assert(ip->Is(*eqv));
	assert(dp->Is(*eqvd));
	assert(sp->Is(*lvq));

	std::unique_ptr<BaseVisitor> av = std::make_unique<AccumulationVisitor<QS>>();
	av = m2e.Accept(std::move(av));
	auto avc = (av->template As<AccumulationVisitor<QS>>());
	assert((avc.Result())==QS(14.00));
	std::cout<<"Visitor"<<avc.Result()<<std::endl;

	std::unique_ptr<BaseVisitor> dv = std::make_unique<DifferenceVisitor<QS>>();
	dv = m2e.Accept(std::move(dv));
	auto vc = (dv->template As<DifferenceVisitor<QS>>());
	assert((*vc())==QS(-1.00));
	std::cout<<"Visitor"<<*vc()<<std::endl;
	
	std::unique_ptr<BaseVisitor> cv = std::make_unique<ConsumptionVisitor<QS>>();
	  M2E m2ce {
	      {std::make_shared<Date>(31,12,2023), std::make_shared<QS>(7.5)},
	      {std::make_shared<Date>(30,9,2023), std::make_shared<QS>(7.5)},
	      {std::make_shared<Date>(4,12,2022), std::make_shared<QS>(5.5)},
	      {std::make_shared<Date>(4,9,2023), std::make_shared<QS>(4.5)},
	      {std::make_shared<Date>(3,5,2021), std::make_shared<QS>(3.5)},
	      {std::make_shared<Date>(3,7,2023), std::make_shared<QS>(3.5)},
	      {std::make_shared<Date>(4,8,2022), std::make_shared<QS>(1.5)},
	  };
	cv = m2ce.Accept(std::move(cv));

	auto diff = ((cv->template As<ConsumptionVisitor<QS>>()))(Year{2023});

	assert(*diff==QS(2));
	
	std::unique_ptr<BaseVisitor> cvv = std::make_unique<ConsumptionVisitor<QV>>();
	  M2E mv {
	      {std::make_shared<Date>(31,12,2023), std::make_shared<QV>(5)},
	      {std::make_shared<Date>(30,9,2023), std::make_shared<QV>(4)},
	      {std::make_shared<Date>(4,12,2022), std::make_shared<QV>(2)},
	      {std::make_shared<Date>(4,9,2023), std::make_shared<QV>(3)},
	      {std::make_shared<Date>(31,6,2023), std::make_shared<QV>(2.5)},
	      {std::make_shared<Date>(3,5,2022), std::make_shared<QV>(1)},
	      {std::make_shared<Date>(3,5,2021), std::make_shared<QV>(0.5)},
	  };
	cvv = mv.Accept(std::move(cvv));

	auto qv = ((cvv->template As<ConsumptionVisitor<QV>>()))(Year{2023},Quantity<Time,Days,uint>(480));
	assert(*qv==QV(4));
	qv = ((cvv->template As<ConsumptionVisitor<QV>>()))(Year{2023},Quantity<Time,Days,uint>(180));
	std::cout<<"Consumption Visitor _>\n"<<*qv<<std::endl;
	assert(*qv==QV(2.5));
	qv = ((cvv->template As<ConsumptionVisitor<QV>>()))(Year{2023});
	assert(*qv==QV(3));

	qv = ((cvv->template As<ConsumptionVisitor<QV>>()))(Year{2022});
	assert(*qv==QV(1.5));
	  
	M2E ma {
	      {std::make_shared<QA>(24), std::make_shared<QS>(45)},
	      {std::make_shared<QA>(18), std::make_shared<QS>(50)},
	      {std::make_shared<QA>(7*12), std::make_shared<QS>(200)},
	      {std::make_shared<QL>(7), std::make_shared<QL>(7)},
	  };
	
	std::unique_ptr<BaseVisitor> fv = std::make_unique<FuncVisitor<QS,QA, Div>>();
	
	fv = ma[0].Accept(std::move(fv));
	auto fV = fv->template As<FuncVisitor<QS,QA, Div>>();
	assert(fV().Value()==1.875);
	
	fv = ma[1].Accept(std::move(fv));
	fV = fv->template As<FuncVisitor<QS,QA, Div>>();
	//assert(fV().Value()==2.77778);
	
	fv = ma[2].Accept(std::move(fv));
	fV = fv->template As<FuncVisitor<QS,QA, Div>>();
	//assert(fV().Value()==2.38095);
	
	std::unique_ptr<BaseVisitor> fl = std::make_unique<FuncVisitor<QL, QL, Mul>>();
	fl = ma.Accept(std::move(fl));
	auto fL = fl->template As<FuncVisitor<QL, QL,Mul>>();
	std::cout<<"Consumption Visitor _>\n"<<fL<<std::endl;
	assert(fL().Value()==49);
	
	std::unique_ptr<BaseVisitor> fc = std::make_unique<ComposedFuncVisitor<QS, FuncVisitor<QL,QL,Mul>,Mul>>();
	fc = ma[3].Accept(std::move(fc));
	auto fC = fc->template As<ComposedFuncVisitor<QS, FuncVisitor<QL,QL,Mul>,Mul>>();
	std::cout<<"Consumption Visitor _>\n"<<fC<<std::endl;
	
	std::cout<<"END Visitor"<<*diff<<std::endl;
   
	return 0;
}
};
