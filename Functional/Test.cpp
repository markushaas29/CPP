#include <iostream>
#include <cassert> 
#include <vector> 
#include <tuple> 
#include <memory> 
#include "Functional.hpp"
#include "../Common/ShortNames.hpp"
#include "../Common/DateTimes.hpp"
#include "../CSV/Elements.hpp"
#include "../Quantity/Quantity.hpp"
using namespace ShortNames;
using namespace DateTimes;

class FunctionalTest
{
	public:
		int Run()
		{
			using QM = Quantity<Mass>;
			using QV = Quantity<Volume>;
			using T = std::tuple<Q,QM>;
			using T3 = std::tuple<Q,QM, QV>;
			using T3_2 = std::tuple<QM, QV, Q>;
			using TR = std::tuple<Year,Month,Day,Quantity<Volume>>;
	
			QM qm{5};
			auto cm = Constant<QM>(qm);
			auto cm2 = Constant(2.0);
			auto cm5 = Constant(5);
			assert(qm==cm());
			assert(2==cm2());
			assert(2==(int)cm2);
			assert(5==(int)cm5);
			assert(5==cm5());
			assert(5==(int)cm);

			auto acm = cm + cm;
			assert((qm+qm)==acm());
			assert((qm+qm)==acm(2));
			auto scm = cm - cm;
			assert((qm-qm)==scm());
			assert((qm-qm)==scm(4));
			auto mcm = cm * cm;
			assert((qm*qm)==mcm());
			assert((qm*qm)==mcm(5));
			auto dcm = cm / cm;
			assert((qm/qm)==dcm());
			assert((qm/qm)==dcm(5));

			std::cout<<"Const "<<cm2<<std::endl;

			auto ac = Add<Constant<QM>, Constant<QM>>(cm,cm);
			assert(ac()==QM{10});
			assert(10==(int)ac);
			std::cout<<"Add "<<ac(5)<<std::endl;
			
			auto ac2 = Add<decltype(ac), Constant<QM>>(ac,cm);
			assert(ac2()==QM{15});
			assert(15==(int)ac2);
			
			auto sc = Sub<decltype(ac2), Constant<QM>>(ac2,cm);
			assert(sc()==QM{10});
			assert(10==(int)sc);
			
			auto mc = Mul<decltype(ac), Constant<QM>>(ac,cm);
			assert(mc()==(QM{5}*QM{10}));
			assert(50==(int)mc);
			
			auto dc = Div<decltype(mc), Constant<QM>>(mc,cm);
			assert(dc()==QM{10});
			assert(10==(int)dc);
		
			auto fd = Func<Div>(mc,cm);
			assert(fd()==QM{10});
			assert(10==(int)fd);
			
			auto fc = Func<Constant>(qm);
			assert(qm==fc());
			assert(5==(int)fc);
			
			auto fp = Func<Parameter>(qm);
			assert(qm==fp());
			assert(qm==fp.Value());
			assert(5==(int)fp);
			
			fp.Value() = QM{2};

			assert(QM{2}==fp());
			assert(QM{2}==fp.Value());
			assert(2==(int)fp);
			
			auto p4 = Func<Parameter>(4);
			assert(4==p4());
			auto cp = Func<Constant>(p4);
			assert(4==cp());

			p4.Value() = 8;
			
			assert(8==p4());
			assert(8==cp());

			auto ccp = cm5 + cp;
			assert(13==ccp());
			
			p4.Value() = 10;
			assert(15==ccp());
			
			auto fx = Func<Fx>();
			assert(0.0==fx());
			assert(2.0==fx(2.0));
			assert(0==(int)fx);
			
			auto afx = fx + fx;
			assert(4==afx(2));
			auto mfx = fx * fx;
			assert(16==mfx(4));
			
			auto p2 = Func<Parameter>(2);
			auto afxp = afx * p2; 
			std::cout<<"Const "<<afxp(2)<<std::endl;
			assert(8==afxp(2));
			p2.Value() = 4;
			std::cout<<"Const "<<afxp(2)<<std::endl;
			assert(16==afxp(2));
			
			auto fx2 = Func<Fx>(qm);
			assert(qm==fx2());
			assert((qm*qm)==fx2(qm*qm));
			assert(5==(int)fx2);

			auto mul = Func<Mul>(Func<Mul>(Func<Mul>(mc,ac), Func<Add>(mc,mc)), Func<Mul>(Func<Mul>(mc,ac), Func<Add>(mc,mc)));
			std::cout<<"Mul "<<mul()<<std::endl;
			std::cout<<"Result "<<decltype(mul)::ResultType()<<std::endl;

			bool is = std::is_same_v<decltype(mul)::LeftType::ResultType, decltype(QM()*QM()*QM()*QM()*QM())>;
			assert(is);

			auto ap = ac + ac;
			auto i = ap();
			std::cout<<"Operator "<<ap<<std::endl;
			std::cout<<"Operator "<<(ac + ac)<<std::endl;
			std::cout<<"Operator "<<(ac + cm)<<std::endl;
			std::cout<<"Operator "<<(cm + ac)<<std::endl;

			std::cout<<"Operator "<<(ac - ac)<<std::endl;
			std::cout<<"Operator "<<(ac - cm)<<std::endl;
			std::cout<<"Operator "<<(cm - ac)<<std::endl;

			std::cout<<"Operator "<<(ac * ac)<<std::endl;
			std::cout<<"Operator "<<(ac * cm)<<std::endl;
			std::cout<<"Operator "<<(cm * ac)<<std::endl;

			std::cout<<"Operator "<<(ac / ac)<<std::endl;
			std::cout<<"Operator "<<(ac / cm)<<std::endl;
			std::cout<<"Operator "<<(cm / ac)<<std::endl;
			std::cout<<"END"<<std::endl;


			std::vector<int> v1 = {1,2,3};
			std::vector<int> v2 = {1,2,3};

			auto acc1 = Acc<decltype(v1),decltype(v2)>(v1,v2);
			assert(acc1()==6);
			
			auto dot1 = Dot<decltype(v1),decltype(v2)>(v1,v2);
			assert(dot1()==14);

			return 0;
		}
};

int main()
{
        FunctionalTest analyzer;

        analyzer.Run();

        return 0;
}

