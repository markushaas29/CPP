#include <iostream>
#include <cassert> 
#include <vector> 
#include <tuple> 
#include <memory> 
#include "../Matrix.hpp"
#include "../MatrixReader.hpp"
#include "../MatrixDescriptor.hpp"
#include "../MatrixProjector.hpp"
#include "../MatrixFilter.hpp"
#include "../MatrixAnalyzer.hpp"
#include "../M3.hpp"
#include "../MatrixCategory.hpp"
#include "../MatrixMultiCategory.hpp"
#include "../../ObjectFactory/Factory.hpp"
#include "../../Common/ShortNames.hpp"
#include "../../Common/DateTimes.hpp"
#include "../../CSV/Elements.hpp"
#include "../../Quantity/Quantity.hpp"
#include "../../Functional/Functional.hpp"
using namespace ShortNames;
using namespace DateTimes;

class MatrixProjectorCalculationTest
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
		
			using QV = Quantity<Volume>;
			using QM = Quantity<Mass>;
			using QS = Quantity<Sum>;
			using QSC = Quantity<Scalar>;
			using T = std::tuple<Q,QM>;
			using T3 = std::tuple<Q,QM, QV>;
			using T3_2 = std::tuple<QM, QV, Q>;
			using TR = std::tuple<Year,Month,Day,Quantity<Volume>>;
		
			using MIQ2 = MatrixProjector<MI2,Q>;
			using MSQ1 = MatrixProjector<MS1,Q>;
			using MST1 = MatrixProjector<MS1,T>;
			using MIT2 = MatrixProjector<MI2,T>;
			using MIT3 = MatrixProjector<MI2,T3>;
			using MST2 = MatrixProjector<MS2,T>;
			using MSC2 = MatrixProjector<MS2,QSC>;
			using MDR2 = MatrixProjector<MD2,TR>;
			
			using MDF2 = MatrixFilter<MD2>;
			using MIF2 = MatrixFilter<MI2>;
			using MSF2 = MatrixFilter<MS2>;
			std::cout<<"START"<<std::endl;
		
			MS1 ms1{
				{std::string("1"),std::string("2")} 
		    };

			MS2 m33s {
	            {"1", "2" ,"3"},
    	        {"4", "5", "6"},
        	    {"7", "8", "9"},
        	};
			
			MS2 m23s {
    	        {"4", "5", "6"},
        	    {"7", "8", "9"},
        	};
		
		 	MI2 m35 {
		        {1, 2, 3, 4, 5},
		        {6, 7, 8, 9,10},
		        {11, 12, 13, 14, 15},
		    };
		 	
			MI2 m33 {
		        {3, 4, 5},
		        {8, 9,10},
		        {13, 14, 15},
		    };
			
			MD2 m33D {
		        {3, 4, 5},
		        {8, 9,10},
		        {13, 14, 15},
		    };
		 	
			MI2 m22 {
		        {1, 2},
				{3, 4}
		    };
			
			auto md22 = m22.To<double>();
			assert((double)md22[1][0]==3.0);
			assert((double)md22[1][1]==4.0);
			
			auto mst22 = m22.To<std::string>();
			std::cout<<"To String:\n"<<(mst22)<<std::endl;
			assert((std::string)mst22[1][0]=="3");
			
			MD2 md2 {
		        {2020, 12, 24, 5123.9},
		        {2019, 11, 23, 5024.9}
		    };
			
			MS2 ms22{
				{std::string("1"),std::string("2")} ,
				{std::string("5"),std::string("6")} 
		    };
  			
			auto rpath = std::string{ "/home/markus/Downloads/CSV_TestFiles_2/SN.csv" };
  			auto mrR = MatrixReader(rpath);
  			auto mR = mrR.M<2>().Cols(3,7,8,9,4,5,6);
			auto mdR = mR.To<double>();
			assert((int)mdR[1][0]==3);
			assert((int)mdR[0][4]==458);

			auto cs = mdR / mR.ColSum();

  			
  			using MPSC = MatrixProjector<decltype(mR),QSC>;
			MPSC mps(mR);
			std::cout<<"Stages:\n"<<mps<<std::endl;
  			
			using QE = Quantity<Work>;
  			using TRF = std::tuple<DateTimes::Date,QE,Entry>;

			auto u22 = std::string{ "/home/markus/Downloads/CSV_TestFiles_2/U_2022.csv" };
			auto u23 = std::string{ "/home/markus/Downloads/CSV_TestFiles_2/U_2023.csv" };
			auto m22r = MatrixReader(u22);
			auto m23r = MatrixReader(u23);

			auto m22S = m22r.M<2>();
			auto m23S = m23r.M<2>();

			auto a22 = MatrixAnalyzer<decltype(m22S)>(m22S);
			auto a23 = MatrixAnalyzer<decltype(m23S)>(m23S);
			auto out22M = a22();
			auto out23M = a23();

			std::vector<typename MatrixInitializer<2,std::string>::MatrixType> mx{out22M, out23M};
			std::vector<MS2> ms3v{m33s, m23s};
			std::vector<MI2> mis{m33, m35};

			auto pfm = std::make_shared<Factory<IMatrixCategory<std::string>>>();
		    pfm->Register("EQ",[](const std::string& s) { return std::make_unique<EquivalenceCat<std::string>>(std::string(s)); });

			auto pfs =  std::make_shared<FactoryStack<IMatrixCategory<std::string>, Factory<IMatrixCategory<std::string>>>>(pfm);
     		pfs->Register("A",[](std::unique_ptr<std::vector<std::unique_ptr<IMatrixCategory<std::string>>>> s) { return std::make_unique<AndCat<std::string>>(std::move(s)); });
			pfs->Register("O",[](std::unique_ptr<std::vector<std::unique_ptr<IMatrixCategory<std::string>>>> s) { return std::make_unique<OrCat<std::string>>(std::move(s)); });

			FactoryUnit<std::string, std::vector<FactoryUnit<std::string, std::string>>> mUO49 = { "O", {{"EQ", "4"}, {"EQ","9"}}};
			
			M3 m3s(ms3v);
			auto m49 = m3s.M(MatrixRowQuery<MS2,std::string>(pfs, mUO49));
			std::cout<<"M3S: \n"<<m3s<<std::endl;
			std::cout<<"M3S: \n"<<m49<<std::endl;

			M3 m3(mx);
			std::cout<<"M3: \n"<<m3<<std::endl;
			std::vector<MI2> mi{m33, m35};

			M3<int> m3i(mis);
			std::cout<<"M3: \n"<<m3i<<std::endl;

			auto out22S = out22M.Col(2).To<QS>();
			auto out23S = out23M.Col(2).To<QS>();
			auto stagS = cs.To<QSC>();
			std::cout<<out23S<<std::endl;
			std::cout<<stagS<<std::endl;

			std::cout<<"Stages total:\n"<<(mR)<<std::endl;
			
			auto sums = stagS*out23S.Rows(1,2,3,4,5);
			std::cout<<sums[0]<<std::endl;
			
			std::cout<<"Stages ratio:\n"<<(cs)<<std::endl;
			
			auto snipath = std::string{ "/home/markus/Downloads/CSV_TestFiles_2/SNI.csv" };
  			auto sniR = MatrixReader(snipath);
			std::cout<<sniR.M<2>()<<std::endl;
			std::cout<<sniR.M<2>()[0][5]<<std::endl;
			std::cout<<sniR.M<2>()[0][5].Split()<<std::endl;

			std::cout<<"END"<<std::endl;
		   
			return 0;
		}
};
