#include <iostream>
#include <fstream>
#include <sstream>
#include <cassert> 
#include <vector> 
#include <memory> 
#include "../Matrix/Matrix.hpp"
#include "../Matrix/MatrixReader.hpp"
#include "../Matrix/MatrixDescriptor.hpp"
#include "../Matrix/MatrixComposition.hpp"
#include "../Matrix/M3.hpp"
#include "../Matrix/MatrixParsers.hpp"
#include "../Matrix/MatrixFormatter.hpp"
#include "../Builder/Builder.hpp"
#include "../Counter/ICounter.hpp"
#include "../ObjectFactory/Factory.hpp"
#include "../Common/DateTimes.hpp"
#include "../CSV/Elements.hpp"
#include "../CSV/Matcher.hpp"
#include "../Quantity/Quantity.hpp"
#include "../Functional/Functional.hpp"
#include "../Common/DateTimes.hpp"
#include "../Visitor/CollectorVisitor.hpp"
#include "../Invoice/InvoiceCalculatorBase.hpp"

#pragma once

template<typename S>
class Readings: public CalculatorBase<Quantity<Volume>, Readings<S>>
{
	using Base = CalculatorBase<Quantity<Volume>, Readings<S>>;
	using Stage = S;
public:
	Readings(std::shared_ptr<Factory<IToken>> fT,std::shared_ptr<Factory<IElement>> fE,std::shared_ptr<Factory<BaseVisitor>> fB, const Year& y,const std::string& p): Base{fE,fB,y}, tokenFactory{fT} {};
private:
	std::shared_ptr<Factory<IToken>> tokenFactory;
	virtual typename Base::QuantityType value() const { return typename Base::QuantityType{0}; };
	typename Base::MatrixType exec(std::shared_ptr<std::ofstream> f) const
	{
		Builder<ICounter,Counter, BottomHotDesc, BottomColdDesc, MiddleHotDesc, MiddleColdDesc,TopHotDesc, TopColdDesc> b;
		auto cV = b("/home/markus/Downloads/CSV_TestFiles_2", tokenFactory);

		auto els = std::vector<std::shared_ptr<IElement>>{};
		
		std::for_each(cV->begin(), cV->end(), [&](const auto& i)
				{ 
        			auto civ = (*Base::visitorFactory)("ConsumptionVolume","");
					civ = i->Accept(std::move(civ));
					auto consV = civ->template As<ConsumptionVisitor<Quantity<Volume>>>();
				//	*f<<consV<<std::endl;
					els.push_back(consV());	
				});

		auto med = Init(els);
		auto readings = med();

        auto accBV = (*Base::visitorFactory)("AccumulationVolume","");
		accBV = readings.Accept(std::move(accBV));

		auto accV = accBV->template As<AccumulationVisitor<Quantity<Volume>>>();
		auto d = Quantity<Volume>((*accV((int)((S::Index-1)*2),(int)(S::Index*2))).template To<Quantity<Volume>>());

	 	auto sum = (*accV()).template To<Quantity<Volume>>();
        std::vector<typename Base::ElementType> resQ = { std::make_shared<Quantity<Scalar>>(d / (*accV()).template To<Quantity<Volume>>()) };

        return Matrix<Base::Order,typename Base::DescriptorType>(typename Base::DescriptorType({resQ.size()}),ToDataType(resQ));
	}
};

template<typename S>
class StageBase: public CalculatorBase<Quantity<Sum>, StageBase<S>>
{
	using Base = CalculatorBase<Quantity<Sum>, StageBase<S>>;
public:
	auto M() const { return parser->M().Rows(0, S::Index); }
protected:
	StageBase(std::shared_ptr<Factory<IToken>> fT,std::shared_ptr<Factory<IElement>> fE,std::shared_ptr<Factory<BaseVisitor>> fB, const Year& y,const std::string& p): 
		Base{fE,fB,y}, path{p}, tokenFactory{fT},parser{std::make_unique<StageParser>(tokenFactory,path)} {};
	const std::string path;
	std::shared_ptr<Factory<IToken>> tokenFactory;
	std::unique_ptr<IMatrixParser<2>> parser;
private:
	const std::string fileName = "SN_Name.csv";
	typename Base::MatrixType exec(std::shared_ptr<std::ofstream> f) const	{	return matrix(f);	}
	virtual typename Base::MatrixType matrix(std::shared_ptr<std::ofstream> f) const = 0;
};

template<typename S>
class ExtraCostItemsCalculator: public StageBase<S>
{
	using Base = StageBase<S>;
public:
	ExtraCostItemsCalculator(std::shared_ptr<Factory<IToken>> fT,std::shared_ptr<Factory<IElement>> fE,std::shared_ptr<Factory<BaseVisitor>> fB, const Year& y,const std::string& p): Base{fT,fE,fB, y, p} {};
private:
	virtual typename Base::MatrixType matrix(std::shared_ptr<std::ofstream> f) const { return (*Base::parser)().Cols(8,9,10)[S::Index-1];	}
};

template<typename S>
class YearlyExtraCostsCalculator: public StageBase<S>
{
	using Base = StageBase<S>;
public:
	YearlyExtraCostsCalculator(std::shared_ptr<Factory<IToken>> fT,std::shared_ptr<Factory<IElement>> fE,std::shared_ptr<Factory<BaseVisitor>> fB, const Year& y,const std::string& p): Base{fT,fE,fB, y,p} {};
private:
	virtual typename Base::MatrixType matrix(std::shared_ptr<std::ofstream> f) const
	{
		auto m = (*(Base::parser))();
		auto mf1 = MatrixFormatter(m[S::Index-1]);
        auto out = mf1(std::to_string(S::Index)+"_S.html","/home/markus/Downloads/CSV_TestFiles_2");
		mf1(*f);
        auto payment = (*(Base::parser))().Cols(8,9,10).template To<Quantity<Sum>>();
        return Matrix<Base::Order, typename Base::DescriptorType>(typename Base::DescriptorType({1}),{std::make_shared<Quantity<Sum>>((payment[S::Index-1][1].template To<Quantity<Sum>>()+payment[S::Index-1][2].template To<Quantity<Sum>>()) * Quantity<Scalar>{12}) });
	}
};

class AccountCalculator: public CalculatorBase<Quantity<Sum>, AccountCalculator>
{
	using Base = CalculatorBase<Quantity<Sum>, AccountCalculator>;
public:
	AccountCalculator(std::shared_ptr<Factory<IToken>> fT,std::shared_ptr<Factory<IElement>> fE,std::shared_ptr<Factory<BaseVisitor>> fB, const Year& y,const std::string& p): Base{fE,fB,y}, parser{std::make_unique<AccountParser>(fT,p)} {};
private:
	std::unique_ptr<IMatrixParser<3>> parser;
	std::unique_ptr<IResult<Quantity<Unit<1>>, Matrix<2, MatrixDescriptor<2,std::shared_ptr<IElement>>>>, std::default_delete<IResult<Quantity<Unit<1>>, Matrix<2, MatrixDescriptor<2, std::shared_ptr<IElement>>>>>> result;
	typename Base::MatrixType exec(std::shared_ptr<std::ofstream> f) const
	{
		using MDS2 = MatrixDescriptor<2,std::string>;
        using MS2 = Matrix<2,MDS2>;
		
		using TF = TypeFactory<CompositeFactory<IPredicateVisitor, Factory<IElement>>, EqualVisitor, LessVisitor>;
		auto typeFactory = std::make_shared<TF>(elementFactory);
		
		auto parsedAccountMatrix = (*parser)();

		//std::cout<<parsedAccountMatrix<<std::endl;

		std::vector<FactoryUnitContainer<std::vector<FactoryUnitContainer<std::vector<FactoryUnit<std::string,FactoryUnit<std::string, std::string>>>>>>> allFactoryUnits = 
        {
            {"Waste",
                {
                
                    {"Deduction",{{"EqualVisitor", { "IBAN", "DE44600501010008017284"}}, {"EqualVisitor", { "Year", "2023"}}}} // Waste
                }
            }, 
            {"Heating",
                {
                    {"Deduction",{{"EqualVisitor", { "Entry", "Abschlagsforderung"}}, {"EqualVisitor", { "Entry", "701006843905"}}, {"EqualVisitor", { "IBAN", "DE56600501017402051588"}}, {"EqualVisitor", { "Year", "2023"}}}},
                    {"Deduction",{{"EqualVisitor", { "Entry", "Abschlagsforderung"}}, {"EqualVisitor", { "IBAN", "DE68600501010002057075"}}, {"EqualVisitor", { "Year", "2023"}}}},
                    {"Invoice",{{"EqualVisitor", { "Entry", "Rechnung"}}, {"EqualVisitor", { "IBAN", "DE56600501017402051588"}}, {"EqualVisitor", { "Year", "2024"}}}},
                    {"Rechnung",{{"EqualVisitor", { "Entry", "Rechnung"}}, {"EqualVisitor", { "IBAN", "DE68600501010002057075"}}, {"EqualVisitor", { "Year", "2024"}}}},
                    //{"Maintenance",{{"EqualVisitor", { "IBAN", "DE73660623660009271112"}}, {"EqualVisitor", { "Year", "2022"}}}},
                }
            },
            {"BuildingInsurance",{
                    {"Invoice",{{"EqualVisitor", { "IBAN", "DE97500500000003200029"}}, {"EqualVisitor", { "Year", "2023"}}}} // Insurance
                }
            },
            {"Cleaning",{
                    {"Alles Proper",{{"EqualVisitor", { "IBAN", "DE05100110012620778704"}}, {"EqualVisitor", { "Year", "2023"}}}},
                    {"Jansen",{{"EqualVisitor", { "IBAN", "DE08548500101700257437"}}, {"EqualVisitor", { "Year", "2023"}}}},
                    {"Jansen",{{"EqualVisitor", { "IBAN", "DE08548500101700257437"}}, {"EqualVisitor", { "Month", "1"}}}},
                    {"Rastaetter",{{"EqualVisitor", { "IBAN", "DE79660623660000101303"}}, {"EqualVisitor", { "Year", "2023"}}}},
                }
            },
            {"PropertyTax",{
                    {"Deduction",{{"EqualVisitor", { "IBAN", "DE12660623660000005703"}}, {"EqualVisitor", { "Year", "2023"}}, {"EqualVisitor", { "Entry", "501000000891/Grundsteuer"}}}} //Grundsteuer
                }
            },
            {"Sewage",{
                    {"Deduction",{{"EqualVisitor", { "IBAN", "DE12660623660000005703"}}, {"EqualVisitor", { "Year", "2023"}}, {"EqualVisitor", { "Entry", "Abschlag/Abwasser"}}}}, //Abwasser
                    {"Invoice",{{"EqualVisitor", { "IBAN", "DE12660623660000005703"}}, {"EqualVisitor", { "Year", "2024"}}, {"EqualVisitor", { "Entry", "Rechnung/Abwasser"}}}} //Abwasser
                }
            }
        };

		auto fu = std::make_shared<std::ofstream>("/home/markus/Downloads/CSV_TestFiles_2/FactoryUnits.txt");
		for(auto u : allFactoryUnits)
			(*fu)<<u;
		fu->close();

		auto all = std::make_unique<MatrixComposite<decltype(parsedAccountMatrix)>>("All");

        std::vector<FactoryUnit<std::string, std::string>> fv{{"AccumulationSum"}};
        for(uint i = 0; i < allFactoryUnits.size(); ++i)
                 all->Add(MatrixComposite<decltype(parsedAccountMatrix)>::Create(typeFactory,visitorFactory,std::move(allFactoryUnits[i].Name()), allFactoryUnits[i].Units(),fv));
        std::unique_ptr<IResult<Quantity<Unit<1>>, Matrix<2, MatrixDescriptor<2,std::shared_ptr<IElement>>>>, std::default_delete<IResult<Quantity<Unit<1>>, Matrix<2, MatrixDescriptor<2, std::shared_ptr<IElement>>>>>> result = (*all)(parsedAccountMatrix);

		(*result)(*f);

		std::cout<<*result<<std::endl;
		std::cout<<"ELEMENTS: "<<result->Elements()<<std::endl;

		return result->Elements();
	}
};

template<typename S>
class ProportionCalculator: public StageBase<S>
{
	using Base = StageBase<S>;
public:
	ProportionCalculator(std::shared_ptr<Factory<IToken>> fT,std::shared_ptr<Factory<IElement>> fE,std::shared_ptr<Factory<BaseVisitor>> fB, const Year& y,const std::string& p): Base{fT,fE,fB, y, p} {};
private:
	virtual typename Base::MatrixType matrix(std::shared_ptr<std::ofstream> f) const 
	{ 
		//*f<<"P"<<(*Base::parser)()[S::Index-1]<<std::endl;
		auto mf = MatrixFormatter((*Base::parser)(true)[S::Index-1]);
        auto out = mf(std::to_string(S::Index)+"_A.html","/home/markus/Downloads/CSV_TestFiles_2");
		append(this->M(),out);
		auto stageMatrix = (*Base::parser)().Cols(2,3,4,5,6,7).template To<Quantity<Scalar>>();
		using AllStages = std::tuple<Bottom, Middle, Top>;
		stageMatrix = process<0,AllStages>(stageMatrix,Base::tokenFactory,Base::elementFactory,Base::visitorFactory, Base::path,f);
        auto costs = calcCosts<0,AllStages>(stageMatrix,Base::tokenFactory,Base::elementFactory,Base::visitorFactory, Base::path,f).Rows(S::Index-1);
		return (*Base::parser)()[S::Index-1];	
	}
	template<size_t N, typename Tup>
    auto process(auto& stageMatrix, std::shared_ptr<Factory<IToken>> fT,std::shared_ptr<Factory<IElement>> fE,std::shared_ptr<Factory<BaseVisitor>> fB, const std::string& p, std::shared_ptr<std::ofstream> f) const 
    {
        if constexpr (std::tuple_size<Tup>()==N)
            return stageMatrix;
        else
        {
            using Type = std::tuple_element_t<N,Tup>;
            auto readings = Readings<Type>{fT,fE,fB, Base::year,p};
            stageMatrix = stageMatrix.Set(readings(f)[0].template As<Quantity<Scalar>>(),Type::Index-1,((int)stageMatrix.Cols()-1));
            return process<N+1,Tup>(stageMatrix,fT,fE,fB,p,f);
        }
    }
    
    template<size_t N, typename Tup>
    auto calcCosts(auto stageMatrix, std::shared_ptr<Factory<IToken>> tokenFactory,std::shared_ptr<Factory<IElement>> elementFactory,std::shared_ptr<Factory<BaseVisitor>> visitorFactory, const std::string& path, std::shared_ptr<std::ofstream> f) const
    {
        auto account = AccountCalculator{tokenFactory,elementFactory,visitorFactory, Base::year, path}; 
        stageMatrix = process<0,Tup>(stageMatrix,tokenFactory,elementFactory,visitorFactory, path, f);

		auto mf = MatrixFormatter(stageMatrix[S::Index-1]);
        auto out = mf(std::to_string(S::Index)+".html","/home/markus/Downloads/CSV_TestFiles_2");

        auto sumMatrix = account(out).template To<Quantity<Sum>>();  
        auto csum = stageMatrix.ColSum()();
        auto stagesDiv = (stageMatrix / csum());
//		*f<<"T\t"<<"Sum\t"<<"Calculation\t"<<"CalcRes\t"<<"\t"<<"\t"<<std::endl;
		auto result = stagesDiv[S::Index-1] * sumMatrix;
		auto res = result().template To<Quantity<Sum>>();
		std::vector<std::vector<std::string>> v = {{"Proportion","All","Calculation","CalcResult","","Out","Payment"}};
		for(size_t i = 0; i < 6; ++i)
		{
			std::vector<std::string> vr;
			expand(stageMatrix[S::Index-1][i],vr);
			expand(csum[i], vr);
			expand(stagesDiv[S::Index-1][i],vr);
			expand(stagesDiv[S::Index-1][i](),vr);
			expand(sumMatrix[i],vr);
			expand(result[i][i],vr);
			expand(res[i][i],vr);
			v.push_back(vr);
		}

		append(Init(v)(),out);
		append(this->M(),out);

//		for(size_t i = 0; i < 6; ++i)
//			*f<<stageMatrix[S::Index-1][i]<<"\t"<<csum[i]<<"\t"<<stagesDiv[S::Index-1][i]<<"\t"<<stagesDiv[S::Index-1][i]()<<"\t"<<result[i][i]<<"\t"<<res[i][i]<<std::endl;
        return stagesDiv * sumMatrix;                                                                                                       
    }

	auto expand(const auto& val, auto& v) const
	{
		std::stringstream ss;
		ss<<val;
		v.push_back(ss.str());
	}
	
	auto append(const auto& m, std::shared_ptr<std::ofstream> out) const
	{
		auto mf = MatrixFormatter(m);
		mf(*out);
	}
};
