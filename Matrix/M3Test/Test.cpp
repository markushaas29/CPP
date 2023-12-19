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
#include "../MatrixVisitor.hpp"
#include "../MatrixQuery.hpp"
#include "../MatrixMultiCategory.hpp"
#include "../MatrixStrategy.hpp"
#include "../../ObjectFactory/Factory.hpp"
#include "../../Common/DateTimes.hpp"
#include "../../CSV/Elements.hpp"
#include "../../CSV/Matcher.hpp"
#include "../../Quantity/Quantity.hpp"
#include "../../Functional/Functional.hpp"
#include "../../Common/DateTimes.hpp"

class M3Test
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
			using Q = Quantity<Sum>;
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
  			using TRF = std::tuple<Date,QE,Entry>;

			auto u22 = std::string{ "/home/markus/Downloads/CSV_TestFiles_2/U_2022.csv" };
			auto u23 = std::string{ "/home/markus/Downloads/CSV_TestFiles_2/U_2023.csv" };
			auto m22r = MatrixReader(u22);
			auto m23r = MatrixReader(u23);

			auto m22S = m22r.M<2>();
			auto m23S = m23r.M<2>();

			std::vector<MS2> ms3v{m33s, m23s};
			std::vector<MI2> mis{m33, m35};

			auto pfm = std::make_shared<Factory<IMatrixCategory<std::string>>>();
		    pfm->Register("EQ",[](const std::string& s) { return std::make_unique<EquivalenceCat<std::string>>(std::string(s)); });
		    pfm->Register("C",[](const std::string& s) { return std::make_unique<HasCat<std::string>>(std::string(s)); });

			auto pfs =  std::make_shared<FactoryStack<IMatrixCategory<std::string>, Factory<IMatrixCategory<std::string>>>>(pfm);
     		pfs->Register("A",[](std::unique_ptr<std::vector<std::unique_ptr<IMatrixCategory<std::string>>>> s) { return std::make_unique<AndCat<std::string>>(std::move(s)); });
			pfs->Register("O",[](std::unique_ptr<std::vector<std::unique_ptr<IMatrixCategory<std::string>>>> s) { return std::make_unique<OrCat<std::string>>(std::move(s)); });

			FactoryUnit<std::string, std::vector<FactoryUnit<std::string, std::string>>> mUO49 = { "O", {{"EQ", "4"}, {"EQ","9"}}};
			
			M3 m3s(ms3v);
			auto m49 = m3s.M(MatrixRowQuery<MS2,std::string>(pfs, mUO49));
			std::cout<<"M3S: \n"<<m3s<<std::endl;
			std::cout<<"M3S: \n"<<m33s.Cols()<<std::endl;
			std::cout<<"M3S: \n"<<m3s.Cols()<<std::endl;
			std::cout<<"M3S: \n"<<m49<<std::endl;
			assert(m33s.Cols()==3);
			assert(m3s.Cols()==3);

			auto t = false;
			try	{ M3<int> m3i(mis);	}catch(...){ t = true;	}
			assert(t);

			std::vector<MS2> m22_23v{m22S, m23S};
			M3 m22_23(m22_23v);
 			
			FactoryUnit<std::string, std::vector<FactoryUnit<std::string, std::string>>> fUDetAb = { "A",  {{"EQ", "DE12660623660000005703"}, {"C","2023"}, {"C","Rechnung/Abwasser"}}}; 
            FactoryUnit<std::string, std::vector<FactoryUnit<std::string, std::string>>> fUDetAb2 = { "A",  {{"EQ", "DE12660623660000005703"}, {"C","2022"}, {"C","Abschlag/Abwasser"}}}; 
            auto mDA = MatrixQuery<decltype(m22S),std::string>(pfs, {fUDetAb, fUDetAb2});
            auto mSewage =m22_23.M(mDA).Cols(4,6,7,9,11);
            std::cout<<"MatrixQuery a:\n"<<mSewage<<std::endl;
            assert(mSewage.Rows()==6);   
            assert(Quantity<Sum>(mSewage.ColSum(4))==Quantity<Sum>(-933.29));

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

            FactoryUnit<std::string, std::vector<FactoryUnit<std::string, std::string>>> fUBC = { "A",  {{"EQ", "DE05100110012620778704"}, {"C","2022"}}}; 
            auto mBC = MatrixQuery<decltype(m22S),std::string>(pfs, {fUBC});
            auto mCleaning =m22_23.M(mBC).Cols(4,6,7,9,11);
            assert(mCleaning.Rows()==3);
			auto mcP = mCleaning.Cols(0,2,4).Parse(matcher);
            std::cout<<"MatrixQuery a:\n"<<Quantity<Sum>(mCleaning.ColSum(4))<<Quantity<Sum>(-214.20)<<std::endl;
//            assert(Quantity<Sum>(mCleaning.ColSum(4))==Quantity<Sum>(-214.20));
            
			FactoryUnit<std::string, std::vector<FactoryUnit<std::string, std::string>>> fUDetG = { "A",  {{"EQ", "DE12660623660000005703"}, {"C","2022"}, {"C","Grundsteuer"}}}; 
            auto mDG = MatrixQuery<decltype(m22S),std::string>(pfs, {fUDetG});
            auto mPropertyTax =m22_23.M(mDG).Cols(4,6,7,9,11);
            assert(mPropertyTax.Rows()==4);
            assert(Quantity<Sum>(mPropertyTax.ColSum(4))==Quantity<Sum>(-423.01));
            std::cout<<"MatrixQuery a:\n"<<mPropertyTax<<std::endl;
			
            FactoryUnit<std::string, std::vector<FactoryUnit<std::string, std::string>>> fUW  = { "A",  {{"EQ", "DE44600501010008017284"}, {"C","2022"}}}; 
            auto mW = MatrixQuery<decltype(m22S),std::string>(pfs, {fUW});
            auto mWasteFees =m22_23.M(mW).Cols(4,6,7,9,11);
            assert(mWasteFees.Rows()==2);
            assert(Quantity<Sum>(mWasteFees.ColSum(4))==Quantity<Sum>(-322.0));
            std::cout<<"MatrixQuery a:\n"<<mWasteFees<<std::endl;
            
			FactoryUnit<std::string, std::vector<FactoryUnit<std::string, std::string>>> fUI  = { "A",  {{"EQ", "DE97500500000003200029"}, {"C","2022"}}}; 
            auto mBI = MatrixQuery<decltype(m22S),std::string>(pfs, {fUI});
            auto mBuildingInsurance =m22_23.M(mBI).Cols(4,6,7,9,11);
            std::cout<<"MatrixQuery a:\n"<<mBuildingInsurance<<std::endl;
            assert(mBuildingInsurance.Rows()==1);
            assert(Quantity<Sum>(mBuildingInsurance.ColSum(4))==Quantity<Sum>(-1671.31));
			
			FactoryUnit<std::string, std::vector<FactoryUnit<std::string, std::string>>> fUZie  = { "A",  {{"EQ", "DE10660501011022126625"}, {"C", "Miete"},{"C","2022"}}}; 
			FactoryUnit<std::string, std::vector<FactoryUnit<std::string, std::string>>> fUZei  = { "A",  {{"C", "Zeiher"}, {"C", "Miete"},{"C","2022"}}}; 
            auto mZ = MatrixQuery<decltype(m22S),std::string>(pfs, {fUZie, fUZei});
            auto mZeiher =m22_23.M(mZ).Cols(4,6,7,9,11);
            std::cout<<"MatrixQuery a:\n"<<mZeiher<<std::endl;
            assert(mZeiher.Rows()==12);
            assert(Quantity<Sum>(mZeiher.ColSum(4))==Quantity<Sum>(9000));
			
			FactoryUnit<std::string, std::vector<FactoryUnit<std::string, std::string>>> fUBru  = { "A",  {{"EQ", "DE83660623660009262008"}, {"C", "MIETE"},{"C","2022"}}}; 
            auto mB = MatrixQuery<decltype(m22S),std::string>(pfs, {fUBru});
            auto mBrustat =m22_23.M(mB).Cols(4,6,7,9,11);
            std::cout<<"MatrixQuery a:\n"<<mBrustat<<std::endl;
            assert(mBrustat.Rows()==12);
            assert(Quantity<Sum>(mBrustat.ColSum(4))==Quantity<Sum>(7720));
			
			FactoryUnit<std::string, std::vector<FactoryUnit<std::string, std::string>>> fUProp = { "A",  {{"EQ", "DE05100110012620778704"}, {"C","2023"}}}; 
			FactoryUnit<std::string, std::vector<FactoryUnit<std::string, std::string>>> fUJansen = { "A",  {{"EQ", "DE08548500101700257437"}, {"C","2023"}}}; 
			FactoryUnit<std::string, std::vector<FactoryUnit<std::string, std::string>>> fURast = { "A",  {{"EQ", "DE79660623660000101303"}, {"C","2023"}}}; 
            auto mBC23 = MatrixQuery<decltype(m22S),std::string>(pfs, {fUProp, fUJansen, fURast});
            mCleaning =m22_23.M(mBC23).Cols(4,6,7,9,11);
            std::cout<<"MatrixQuery Cleaninga:\n"<<mCleaning<<std::endl;
            std::cout<<"MatrixQuery a:\n"<<Quantity<Sum>(mCleaning.ColSum(4)).Value()<<Quantity<Sum>(-446.57).Value()<<std::endl;
            double s = Quantity<Sum>(mCleaning.ColSum(4)).Value();
			double q=  Quantity<Sum>(-446.57).Value();
			std::cout<<s<<q<<(s==q)<<std::endl;
            //assert(s==q);
            
			FactoryUnit<std::string, std::vector<FactoryUnit<std::string, std::string>>> fUEnBW = { "A",  {{"EQ", "DE56600501017402051588"}, {"C","2022"}, {"C","Strom Abschlagsforderung"}, {"C","701006843905"}}}; 
            FactoryUnit<std::string, std::vector<FactoryUnit<std::string, std::string>>> fUErdgas = { "A",  {{"EQ","DE68600501010002057075"}, {"C","2022"}, {"C","Gas Abschlagsforderung"}, {"C","8201090081"}}};
            FactoryUnit<std::string, std::vector<FactoryUnit<std::string, std::string>>> fUErdgasInv = { "A",  {{"EQ","DE68600501010002057075"}, {"C","2023"}, {"C","Rechnung"}, {"C","8201090081"}}};
			FactoryUnit<std::string, std::vector<FactoryUnit<std::string, std::string>>> fUEnBWInv = { "A",  {{"EQ", "DE56600501017402051588"}, {"C","2023"}, {"C","Rechnung"}, {"C","701006843905"} }}; 
			auto mq39 = MatrixQuery<decltype(m22S),std::string>(pfs, {fUEnBW, fUErdgas, fUEnBWInv, fUErdgasInv});
            auto Heating =m22_23.M(mq39).Cols(4,6,7,9,11);
			mcP = Heating.Cols(0,2,4).Parse(matcher);
            std::cout<<"MatrixQuery:\n"<<m22_23.M(mq39)<<std::endl;
			assert(Heating.Rows()==25);
            std::cout<<"MatrixQuery:\n"<<Heating.ColSum(4)<<Heating.Col(4)<<std::endl;
            //-2113 -510 +83.94 +490.83
			assert(Quantity<Sum>(Heating.ColSum(4))==Quantity<Sum>(-2048.23));
            std::cout<<"MatrixQuery:\n"<<Heating<<std::endl;

			Year y{2022};
			FactoryUnit<std::string, std::vector<FactoryUnit<std::string, std::string>>> fU_EnBW = { "A",  {{"EQ", "DE56600501017402051588"}, {"C","Strom Abschlagsforderung"}, {"C","701006843905"}}}; 
            FactoryUnit<std::string, std::vector<FactoryUnit<std::string, std::string>>> fU_Erdgas = { "A",  {{"EQ","DE68600501010002057075"},  {"C","Gas Abschlagsforderung"}}};
            FactoryUnit<std::string, std::vector<FactoryUnit<std::string, std::string>>> fU_ErdgasInv = { "A",  {{"EQ","DE68600501010002057075"},  {"C","Rechnung"}, {"C","8201090081"}}};
			FactoryUnit<std::string, std::vector<FactoryUnit<std::string, std::string>>> fU_EnBWInv = { "A",  {{"EQ", "DE56600501017402051588"}, {"C","Rechnung"}, {"C","701006843905"} }}; 
			YearStrategy<decltype(m22S)> un( pfs,{fU_EnBW, fU_Erdgas, fU_EnBWInv, fU_ErdgasInv}, y, "BuildingInsurance");
			assert(un.Name()=="BuildingInsurance");
			auto qms = un(m22S);
			
			FactoryUnit<std::string, std::vector<FactoryUnit<std::string, std::string>>> fU_DetG = { "A",  {{"EQ", "DE12660623660000005703"}, {"C","Grundsteuer"}}}; 
			YearStrategy<decltype(m22S)> ys( pfs,{fU_DetG}, y,"BuildingInsurance");
			std::cout<<"ID "<<ys(m22S)<<std::endl;
            assert(ys(m22S).Result()==Quantity<Sum>(-423.01));
            assert(m22S.M(ys).Result()==Quantity<Sum>(-423.01));

			auto id = std::string("8201090081");
			IDStrategy<decltype(m22S)> ids(pfs,{fU_Erdgas}, y, id,"Gas");
			std::cout<<"ID "<<ids(m22S)<<std::endl;
            assert(ids(m22S).Result()==Quantity<Sum>(-2113.00));
            
			auto msR = m22_23.M(ids);

			auto msR2 = msR.Compose(msR);

			std::cout<<"QMS\n\n"<<msR<<std::endl;
            assert(ys(m22S).Result()==Quantity<Sum>(-423.01));
			std::cout<<"Compose\n\n"<<msR2<<std::endl;
			
			BaseMatrixStrategy<decltype(m22S)> ws( pfs,{fU_DetG}, "With");
			auto mb = ws.With(y);
			std::cout<<"ID "<<mb(m22S)<<std::endl;
            assert(m22S.M(mb).Result()==Quantity<Sum>(-423.01));
		
			assert(mcP.Col(0).To<Date>()[0]()==Date(16,12,2022));
			assert(mcP.Col(0)[0].As<Date>()==Date(16,12,2022));
			
			assert(mcP.Col(2).To<Quantity<Sum>>()[0]()==Quantity<Sum>(-48));
			assert(mcP.Col(2)[0].As<Quantity<Sum>>()==Quantity<Sum>(-48));

			MatrixVisitor<TransferVisitor> mv;
			mcP.Accept(mv);

			std::cout<<"Collector:\n"<<mv<<std::endl;
			std::cout<<"Collector T:\n"<<mv.Collector().Total()<<std::endl;
			auto ivs = std::string("DE56600501017402051588");
			auto iban = IBAN(ivs);
			auto f = [](auto l, auto r) { return l.template To<IBAN>() == r; };
			std::vector<std::unique_ptr<IPredicateVisitor>> vip;// = { std::make_unique<LessVisitor>(std::make_unique<Quantity<Sum>>(-40))};
			auto resv = mcP.Accept(std::move(vip));
			std::cout<<"M3 D2 RES:"<<resv<<std::endl;
			auto vib = mv.Collector().Sort<EqualVisitor>(LessVisitor(std::make_unique<Quantity<Sum>>(-40)),Unique<IBAN>());
			auto vib2 = mv.Collector().Get(4.1);
			std::cout<<"M3 D:"<<vib<<std::endl;
			std::cout<<"M3 D2:"<<vib2<<std::endl;
//			auto vs = mv.Collector().Sort(f,Less<Quantity<Sum,Pure,double>>(Quantity<Sum,Pure,double>(-42.0)));
//			for(auto d : vs)
//				std::cout<<"M3 D:"<<d<<std::endl;
//			auto vd = mv.Collector().Sort(f,Less<Date>(Date(1,7,2022)));
//			for(auto d : vd)
//				std::cout<<"M3 D:"<<d<<std::endl;

			auto vi = std::vector<std::unique_ptr<IToken>>();
            vi.push_back(std::make_unique<DateIndexToken>());
            auto r23 = m23S.Cols(vi).Rows();
            auto r22 = m22S.Cols(vi).Rows();
            //std::cout<<"COL"<<m23S.Cols(vi)<<std::endl;
			//
			auto r22_23 = m22_23.Cols(vi).Rows();
			assert(r22_23==(r22+r23));

			std::vector<size_t> v024 = {0,2,4};
            auto c024 = m22_23.Cols(v024);
			assert(r22_23==c024.Rows());
			assert(3==c024.Cols());
            auto a024 = m22_23.Cols(std::array<size_t,2>{1,4});
			assert(r22_23==c024.Rows());
			assert(4==a024.Cols());
            auto i024 = m22_23.Cols(0,2,4,3,1);
			assert(r22_23==i024.Rows());
			assert(5==i024.Cols());

			std::cout<<"END"<<std::endl;
		   
			return 0;
		}
};
