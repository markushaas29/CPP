#include <iostream>
#include <cassert> 
#include <vector> 
#include <tuple> 
#include <memory> 
#include "../Matrix.hpp"
#include "../MatrixReader.hpp"
#include "../MatrixDescriptor.hpp"
#include "../MatrixInitializer.hpp"
#include "../MatrixAnalyzer.hpp"
#include "../MatrixFilter.hpp"
#include "../MatrixFilters.hpp"
#include "../MatrixStrategy.hpp"
#include "../MatrixCategory.hpp"
#include "../MatrixMultiCategory.hpp"
#include "../Factory.hpp"
#include "../../Common/ShortNames.hpp"
#include "../../CSV/Elements.hpp"
#include "../../Quantity/Quantity.hpp"
#include "../../ObjectFactory/Factory.hpp"
using namespace ShortNames;

class MatrixQueryTest
{
	public:
		int Run()
		{
			std::cout<<"START"<<std::endl;
		
			using M3 = Matrix<3>;
			using M2 = Matrix<2>;
			using MS3 = MatrixDescriptor<3,double>;
			using MS2 = MatrixDescriptor<2,std::string>;
			using MS1 = MatrixDescriptor<1,std::string>;
			using MQ1DS = MatrixDescriptor<1,double>;
			using M3D = Matrix<3,MS3>;
			using M1S = Matrix<1,MS1>;
			using M1Q = Matrix<1,MQ1DS>;
			using M2M = Matrix<2,MS2>;
			using M1 = Matrix<1>;
			
			M1 m3 {
            	{3, 4, 5},
            };
			M2 m33 {
            	{3, 4, 5},
                {8, 9, 5},
                {13, 14, 15},
            };

			std::vector<int> v1{1,2,3,4};
			std::vector<int> v1e{1,1,1,1};
			std::vector<std::vector<int>> v2{{1,2,3},{4,5,6}};
			std::vector<std::vector<std::vector<int>>> v3{{{1},{2}},{{3},{4}}};
			std::vector<std::vector<std::vector<int>>> v33{{{1,1,1},{2,2,2}},{{3,3,3},{4,4,4}},{{3,3,3},{4,4,4}}};
		
			auto u22 = std::string{ "/home/markus/Downloads/CSV_TestFiles_2/U_2022.csv" };
			auto u23 = std::string{ "/home/markus/Downloads/CSV_TestFiles_2/U_2023.csv" };
			auto m22r = MatrixReader(u22);
			auto m23r = MatrixReader(u23);

			auto m22 = m22r.M<2>();
			auto m23 = m23r.M<2>();

			auto s23 = UnaryMatrixStrategy<decltype(m23)>("");
			s23(m23);

			std::vector<std::unique_ptr<IMatrixStrategy<decltype(m23)>>> strat;
			strat.push_back(std::make_unique<UnaryMatrixStrategy<decltype(m23)>>("")); 
			strat.push_back(std::make_unique<BinaryMatrixStrategy<decltype(m23)>>(std::vector<std::string>()));
			strat.push_back(std::make_unique<BinaryMatrixStrategy<decltype(m23)>>(std::vector<std::string>()));

			(*strat[0])(m23);
			(*strat[1])(m23);

			EquivalenceCat<int> eq(5);
			EquivalenceCat<int> eqs("5");
			EquivalenceCat<double> eqd("7.5");
			try{ EquivalenceCat<double> eqde("abc"); } catch(...) {};

			auto peq = std::make_unique<EquivalenceCat<int>>(5);
			auto peq5 = std::make_unique<EquivalenceCat<int>>(5);
			auto peq8 = std::make_unique<EquivalenceCat<int>>(8);
			auto vc =std::make_unique<std::vector<std::unique_ptr<IMatrixCategory<int>>>>();
			vc->push_back(std::move(peq5));
			vc->push_back(std::move(peq8));
			
			EquivalenceCat<int> eq6(6);
			assert(eq(5));
			assert(eqs(5));
			assert(eqd(7.5));
			assert(decltype(eq)::TypeId=="Equivalence");
			auto s1 = std::string("Equivalence");
			auto s2 = std::string("");
			//Create(m22,s1,s2);
			assert(!eq(4));
			auto s6 = std::string("6");
			ContainCat cc6(s6);
			
			OrCat<int> mc(std::move(vc));
			assert(mc.Size()==2);
			assert(mc(5));
			assert(mc(8));
			assert(!mc(12));
			
			auto peq13 = std::make_unique<EquivalenceCat<int>>(13);
			auto peq14 = std::make_unique<EquivalenceCat<int>>(14);
			auto vsc =std::make_unique<std::vector<std::unique_ptr<IMatrixCategory<int>>>>();
			vsc->push_back(std::move(peq13));
			vsc->push_back(std::move(peq14));

			AndCat<int> msc(std::move(vsc));
			assert(msc.Size()==2);
			assert(!msc(13));
			assert(!msc(14));
			assert(msc());
			assert(msc.Reset());
			assert(!msc(13));
			assert(!msc());

			MatrixFilter<M2> filter(m33);
			auto fr = filter(2,eq);
			assert(fr.Rows()==2);

			auto mq = MatrixQuery<M2>(std::move(peq));
			auto mcq1 = MatrixColQuery<M2>(1,std::make_unique<EquivalenceCat<int>>(5));
			auto m1q5 = MatrixQuery<M1>(std::make_unique<EquivalenceCat<int>>(5));
			auto mc1q5 = MatrixQuery<M1>(std::make_unique<EquivalenceCat<int>>(5));
			auto m1q6 = MatrixQuery<M1>(std::make_unique<EquivalenceCat<int>>(6));
			auto mcq2 = MatrixColQuery<M2>(2,std::make_unique<EquivalenceCat<int>>(5));
			
			auto peq3 = std::make_unique<EquivalenceCat<int>>(3);
			auto peq4 = std::make_unique<EquivalenceCat<int>>(4);
			auto vc34 =std::make_unique<std::vector<std::unique_ptr<IMatrixCategory<int>>>>();
			vc34->push_back(std::move(peq3));
			vc34->push_back(std::move(peq4));
			std::unique_ptr<OrCat<int>> pmc34 = std::unique_ptr<OrCat<int>>( new OrCat<int>(std::move(vc34)));
			auto mrq2 = MatrixRowQuery<M2>(std::move(pmc34));

			auto peq15 = std::make_unique<EquivalenceCat<int>>(13);
			auto v15 =std::make_unique<std::vector<std::unique_ptr<IMatrixCategory<int>>>>();
			v15->push_back(std::move(peq15));
			std::unique_ptr<AndCat<int>> pmsc15 = std::unique_ptr<AndCat<int>>( new AndCat<int>(std::move(v15)));
			auto mrq15 = MatrixRowQuery<M2>(std::move(pmsc15));
			auto m15 = m33.M(mrq15);
			assert(m15.Rows()==1);
			assert(m15[0][2].To<int>()==15);
		
			peq3 = std::make_unique<EquivalenceCat<int>>(3);
			peq5 = std::make_unique<EquivalenceCat<int>>(5);
			auto v10 =std::make_unique<std::vector<std::unique_ptr<IMatrixCategory<int>>>>();
			v10->push_back(std::move(peq3));
			v10->push_back(std::move(peq5));
			std::unique_ptr<AndCat<int>> pms10 = std::unique_ptr<AndCat<int>>( new AndCat<int>(std::move(v10)));
			auto mrq10 = MatrixRowQuery<M2>(std::move(pms10));
			auto m10 = m33.M(mrq10);
			std::cout<<"ADD"<<m10<<std::endl;
			assert(m10.Rows()==1);
			assert(m10[0][2].To<int>()==5);
			assert(m10[0][0].To<int>()==3);
			
			auto mm2 = m33.M(mrq2);

			auto cr = m33.M(mq);
			assert(cr.Rows()==2);

			auto cr1 = m3.M(m1q5);
			assert(cr1.Rows()==1);
			
			auto ccr1 = m3.M(mc1q5);
			assert(ccr1.Rows()==1);
			
			auto cr6 = m3.M(m1q6);
			assert(cr6.Rows()==0);
			
			cr = m33.M(mcq1);
			assert(cr.Rows()==0);
			cr = m33.M(mcq2);
			assert(cr.Rows()==2);
			
			auto mA33 = m33 + m33;
			assert((int)mA33[0][0]==6);
			auto mAq = MatrixQuery<decltype(mA33),int>(std::make_unique<EquivalenceCat<int>>(6));
			auto M6 = mA33.M(mAq);
			std::cout<<"EQ"<<eq<<std::endl;
			assert(M6.Rows()==1);
		
			std::vector<FactoryUnit<std::string, std::string>> units = { {"C", "DE12660623660000005703"}};
		    auto pf = std::make_shared<Factory<IMatrixCategory<std::string>>>();                                                    
		    pf->Register("EQ",[](const std::string& s) { return std::make_unique<EquivalenceCat<std::string>>(std::string(s)); });
		    pf->Register("C",[](std::string_view s) { return std::make_unique<ContainCat<std::string>>(std::string(s)); });
		    assert(pf->Size()==2);

			auto cdet = (*pf)(units);
			auto MDet = m22.M(MatrixQuery<decltype(m22),std::string>(std::move(cdet->at(0))));
			std::cout<<"EQ"<<MDet<<std::endl;

//			auto peq10 = std::make_unique<EquivalenceCat<int>>(10);
//			v10 =std::make_unique<std::vector<std::unique_ptr<IMatrixCategory<int>>>>();
//			v10->push_back(std::move(peq10));
//			auto pmsA10 = std::unique_ptr<AndCat<decltype(mA33)>>( new AndCat<decltype(mA33)>(std::move(v10)));
//			mrq10 = MatrixRowQuery<decltype(mA33)>(std::move(pmsA10));
//			m10 = mA33.M(mrq10);
//			assert(m10.Rows()==1);
//			assert(m10[0][2].To<int>()==5);
//			assert(m10[0][0].To<int>()==3);
			auto mq15 = MatrixQuery<M2>(std::make_unique<EquivalenceCat<int>>(15));
			EquivalenceCat<int> eq15(15);
			cr = m33.M(mq15);
			assert(cr.Rows()==1);

			return 0;
		}
};
