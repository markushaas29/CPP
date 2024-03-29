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
#include "../Factory.hpp"
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
			auto peq5 = std::make_unique<EquivalenceCat<int>>(5);
			auto peq8 = std::make_unique<EquivalenceCat<int>>(8);
			auto vc =std::make_unique<std::vector<std::unique_ptr<IMatrixCategory<int>>>>();
			vc->push_back(std::move(peq5));
			vc->push_back(std::move(peq8));
			EquivalenceCat<int> eq6(6);
			assert(eq(5));
			assert(decltype(eq)::TypeId=="Equivalence");
			auto s1 = std::string("Equivalence");
			auto s2 = std::string("");
			//Create(m22,s1,s2);
			assert(!eq(4));

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
			assert(!msc());
			assert(!msc(14));
			assert(msc());
			assert(msc.Reset());
			assert(!msc(13));
			assert(!msc());

			MatrixFilter<M2> filter(m33);
			auto fr = filter(2,eq);
			assert(fr.Rows()==2);

			

			EquivalenceCat<std::string> eqIB("DE56600501017402051588");
			auto peqS = std::make_unique<EquivalenceCat<std::string>>("DE56600501017402051588");
			
			HasCat<std::string> eqR("Proper");
			assert(decltype(eqR)::TypeId=="Has");
			auto peqS2 = std::make_unique<HasCat<std::string>>("Proper");
			
			std::cout<<"EQ "<<eq<<std::endl;

			return 0;
		}
};
