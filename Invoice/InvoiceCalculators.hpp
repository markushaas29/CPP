#include <iostream>
#include <fstream>
#include <cassert> 
#include <vector> 
#include <memory> 
#include "../Matrix/Matrix.hpp"
#include "../Matrix/MatrixReader.hpp"
#include "../Matrix/MatrixDescriptor.hpp"
#include "../Matrix/MatrixComposition.hpp"
#include "../Matrix/M3.hpp"
#include "../Matrix/MatrixParsers.hpp"
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
	Readings(std::shared_ptr<Factory<IToken>> fT,std::shared_ptr<Factory<IElement>> fE,std::shared_ptr<Factory<BaseVisitor>> fB, const std::string& p): Base{fE,fB}, tokenFactory{fT} {};
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
					//std::cout<<"CONS"<<consV<<std::endl;
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
protected:
	StageBase(std::shared_ptr<Factory<IToken>> fT,std::shared_ptr<Factory<IElement>> fE,std::shared_ptr<Factory<BaseVisitor>> fB, const std::string& p): Base{fE,fB}, parser{std::make_unique<StageParser>(fT,p)} {};
	std::unique_ptr<IMatrixParser<2>> parser;
private:
	const std::string fileName = "SN_Name.csv";
	typename Base::MatrixType exec(std::shared_ptr<std::ofstream> f) const	{	return matrix();	}
	virtual typename Base::MatrixType matrix() const = 0;
};

template<typename S>
class ExtraCostItemsCalculator: public StageBase<S>
{
	using Base = StageBase<S>;
public:
	ExtraCostItemsCalculator(std::shared_ptr<Factory<IToken>> fT,std::shared_ptr<Factory<IElement>> fE,std::shared_ptr<Factory<BaseVisitor>> fB, const std::string& p): Base{fT,fE,fB, p} {};
private:
	virtual typename Base::MatrixType matrix() const { return (*Base::parser)().Cols(8,9,10)[S::Index-1];	}
};

template<typename S>
class YearlyExtraCostsCalculator: public StageBase<S>
{
	using Base = StageBase<S>;
public:
	YearlyExtraCostsCalculator(std::shared_ptr<Factory<IToken>> fT,std::shared_ptr<Factory<IElement>> fE,std::shared_ptr<Factory<BaseVisitor>> fB, const std::string& p): Base{fT,fE,fB, p} {};
private:
	virtual typename Base::MatrixType matrix() const
	{
        auto payment = (*(Base::parser))().Cols(8,9,10).template To<Quantity<Sum>>();
        return Matrix<Base::Order, typename Base::DescriptorType>(typename Base::DescriptorType({1}),{std::make_shared<Quantity<Sum>>((payment[S::Index-1][1].template To<Quantity<Sum>>()+payment[S::Index-1][2].template To<Quantity<Sum>>()) * Quantity<Scalar>{12}) });
	}
};

class AccountCalculator: public CalculatorBase<Quantity<Sum>, AccountCalculator>
{
	using Base = CalculatorBase<Quantity<Sum>, AccountCalculator>;
public:
	AccountCalculator(std::shared_ptr<Factory<IToken>> fT,std::shared_ptr<Factory<IElement>> fE,std::shared_ptr<Factory<BaseVisitor>> fB, const std::string& p): Base{fE,fB}, parser{std::make_unique<AccountParser>(fT,p)} {};
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

		auto all = std::make_unique<MatrixComposite<decltype(parsedAccountMatrix)>>("All");

        std::vector<FactoryUnit<std::string, std::string>> fv{{"AccumulationSum"}};
        for(uint i = 0; i < allFactoryUnits.size(); ++i)
                 all->Add(MatrixComposite<decltype(parsedAccountMatrix)>::Create(typeFactory,visitorFactory,std::move(allFactoryUnits[i].Name()), allFactoryUnits[i].Units(),fv));
        std::unique_ptr<IResult<Quantity<Unit<1>>, Matrix<2, MatrixDescriptor<2,std::shared_ptr<IElement>>>>, std::default_delete<IResult<Quantity<Unit<1>>, Matrix<2, MatrixDescriptor<2, std::shared_ptr<IElement>>>>>> result = (*all)(parsedAccountMatrix);

		*f<<*result;
		*f<<"TEST"<<std::endl;

		return result->Elements();
	}
};

template<size_t N, typename Tup>
auto process(auto& stageMatrix, std::shared_ptr<Factory<IToken>> fT,std::shared_ptr<Factory<IElement>> fE,std::shared_ptr<Factory<BaseVisitor>> fB, const std::string& p)
{
    if constexpr (std::tuple_size<Tup>()==N)
        return stageMatrix;
    else
    {
        using Type = std::tuple_element_t<N,Tup>;
		auto readings = Readings<Type>{fT,fE,fB, p};
		stageMatrix = stageMatrix.Set(readings()[0].template As<Quantity<Scalar>>(),Type::Index,((int)stageMatrix.Cols()-1));
        return process<N+1,Tup>(stageMatrix,fT,fE,fB,p);
    }
}

template<size_t N, typename Tup>
auto calcCosts(auto stageMatrix, std::shared_ptr<Factory<IToken>> tokenFactory,std::shared_ptr<Factory<IElement>> elementFactory,std::shared_ptr<Factory<BaseVisitor>> visitorFactory, const std::string& path)
{
	auto account = AccountCalculator{tokenFactory,elementFactory,visitorFactory, path}; 
	stageMatrix = process<0,Tup>(stageMatrix,tokenFactory,elementFactory,visitorFactory, path);
	
	auto sumMatrix = account().To<Quantity<Sum>>();  
	auto stagesDiv = (stageMatrix / stageMatrix.ColSum());
	return stagesDiv * sumMatrix;                                                                                                       
}

template<size_t N, typename Tup>
auto calcAll(auto stageMatrix, std::shared_ptr<Factory<IToken>> tokenFactory,std::shared_ptr<Factory<IElement>> elementFactory,std::shared_ptr<Factory<BaseVisitor>> visitorFactory, const std::string& path)
{
	stageMatrix = process<0,Tup>(stageMatrix,tokenFactory,elementFactory,visitorFactory, path);
	auto costs = calcCosts<0,Tup>(stageMatrix,tokenFactory,elementFactory,visitorFactory, path).Rows(N+1);

	auto extraCosts = YearlyExtraCostsCalculator<std::tuple_element_t<N,Tup>>{tokenFactory,elementFactory,visitorFactory, path};

	auto e = extraCosts()[0];
	return costs()[0].template To<Quantity<Sum>>() + extraCosts()[0].template As<Quantity<Sum>>();
}
