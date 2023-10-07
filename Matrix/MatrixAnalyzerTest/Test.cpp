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
#include "../../Common/ShortNames.hpp"
#include "../../CSV/Elements.hpp"
#include "../../Quantity/Quantity.hpp"
using namespace ShortNames;

class MatrixAnalyzerTest
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
			
			M2 m33 {
            	{3, 4, 5},
                {8, 9, 5},
                {13, 14, 15},
            };

			std::vector<int> v1{1,2,3,4};
			std::vector<std::vector<int>> v2{{1,2,3},{4,5,6}};
			std::vector<std::vector<std::vector<int>>> v3{{{1},{2}},{{3},{4}}};
			std::vector<std::vector<std::vector<int>>> v33{{{1,1,1},{2,2,2}},{{3,3,3},{4,4,4}},{{3,3,3},{4,4,4}}};
		
			auto u22 = std::string{ "/home/markus/Downloads/CSV_TestFiles_2/U_2022.csv" };
			auto u23 = std::string{ "/home/markus/Downloads/CSV_TestFiles_2/U_2023.csv" };
			auto m22r = MatrixReader(u22);
			auto m23r = MatrixReader(u23);

			auto m22 = m22r.M<2>();
			auto m23 = m23r.M<2>();

			auto a22 = MatrixAnalyzer<decltype(m22)>(m22);
			auto a23 = MatrixAnalyzer<decltype(m23)>(m23);
			a22();
			a23();

			auto s23 = UnaryMatrixStrategy<decltype(m23)>("");
			s23(m23);

			std::vector<std::unique_ptr<IMatrixStrategy<decltype(m23)>>> strat;
			strat.push_back(std::make_unique<UnaryMatrixStrategy<decltype(m23)>>("")); 
			strat.push_back(std::make_unique<BinaryMatrixStrategy<decltype(m23)>>(std::vector<std::string>()));
			strat.push_back(std::make_unique<BinaryMatrixStrategy<decltype(m23)>>(std::vector<std::string>()));

			(*strat[0])(m23);
			(*strat[1])(m23);

			EquivalenceCat<int> eq(5);
			auto peq = std::make_unique<EquivalenceCat<int>>(5);
			EquivalenceCat<int> eq6(6);
			assert(eq(5));
			assert(!eq(4));

			MatrixFilter<M2> filter(m33);
			auto fr = filter(2,eq);
			assert(fr.Rows()==2);

			auto mq = MatrixQuery<M2>(std::move(peq));
			auto mcq1 = MatrixColQuery<M2>(1,std::make_unique<EquivalenceCat<int>>(5));
			auto mcq2 = MatrixColQuery<M2>(2,std::make_unique<EquivalenceCat<int>>(5));
			
			auto cr = m33.M(mq);
			assert(cr.Rows()==2);

			cr = m33.M(mcq1);
			assert(cr.Rows()==0);
			cr = m33.M(mcq2);
			assert(cr.Rows()==2);
			
			auto mA33 = m33 + m33;
			assert((int)mA33[0][0]==6);
			//auto M6 = mA33.M(eq6);
			//assert(M6.Rows()==1);
			
			auto mq15 = MatrixQuery<M2>(std::make_unique<EquivalenceCat<int>>(15));
			EquivalenceCat<int> eq15(15);
			cr = m33.M(mq15);
			assert(cr.Rows()==1);

			EquivalenceCat<std::string> eqIB("DE56600501017402051588");
			auto peqS = std::make_unique<EquivalenceCat<std::string>>("DE56600501017402051588");
			auto mqs = MatrixQuery<M2M>(std::move(peqS));
			auto ib = m22.M(mqs);
			
			ContainCat<std::string> eqR("Proper");
			auto peqS2 = std::make_unique<ContainCat<std::string>>("Proper");
			auto mqs2 = MatrixQuery<M2M>(std::move(peqS2));
			ib = m22.M(mqs2);
			
			std::cout<<"END"<<ib<<std::endl;

			return 0;
		}
};
