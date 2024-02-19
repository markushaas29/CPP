#include <iostream>
#include <cassert> 
#include <vector> 
#include <memory> 
#include "../Matrix.hpp"
#include "../MatrixReader.hpp"
#include "../MatrixDescriptor.hpp"
#include "../MatrixComposition.hpp"
#include "../M3.hpp"
#include "../MatrixParsers.hpp"
#include "../../Builder/Builder.hpp"
#include "../../Counter/ICounter.hpp"
#include "../../ObjectFactory/Factory.hpp"
#include "../../Common/DateTimes.hpp"
#include "../../CSV/Elements.hpp"
#include "../../CSV/Matcher.hpp"
#include "../../Quantity/Quantity.hpp"
#include "../../Functional/Functional.hpp"
#include "../../Common/DateTimes.hpp"
#include "../../Visitor/CollectorVisitor.hpp"

#pragma once

class XBase
{
	inline static constexpr const char TypeIdentifier[] = "Matrix";
    inline static constexpr Literal TypeId{TypeIdentifier};
	template<typename U> using IsT =  Is<U,TypeId>;
public:
    inline static constexpr size_t Order = 1;
	using ElementType = std::shared_ptr<IElement>;
	using DescriptorType = MatrixDescriptor<1,ElementType>;
	using MatrixType = Matrix<1, DescriptorType>;
	MatrixType operator()() const { return exec(); };
protected:
	XBase(std::shared_ptr<Factory<IToken>> fT,std::shared_ptr<Factory<IElement>> fE,std::shared_ptr<Factory<BaseVisitor>> fB):tokenFactory{fT}, elementFactory{fE}, visitorFactory{fB} {};
	std::shared_ptr<Factory<IToken>> tokenFactory;
	std::shared_ptr<Factory<IElement>> elementFactory;
	std::shared_ptr<Factory<BaseVisitor>> visitorFactory;
	const std::string path;
private:
	virtual MatrixType exec() const = 0;
	friend std::ostream& operator<<(std::ostream& s, const XBase& m) { return s; }
};

template<typename S>
class Readings: public XBase
{
	using Base = XBase;
	using Stage = S;
public:
	Readings(std::shared_ptr<Factory<IToken>> fT,std::shared_ptr<Factory<IElement>> fE,std::shared_ptr<Factory<BaseVisitor>> fB, const std::string& p): XBase{fT,fE,fB} {};
private:
	typename Base::MatrixType exec() const
	{
		auto fbv = std::make_shared<Factory<BaseVisitor>>();
        auto reg3 = Registration<Factory<BaseVisitor>,DifferenceVisitor<Quantity<Energy, KiloHour>>,DifferenceVisitor<Date>, AccumulationVisitor<Quantity<Volume>>>(&(*fbv));

		Builder<ICounter,Counter,TopHotDesc, TopColdDesc, MiddleHotDesc, MiddleColdDesc, BottomHotDesc, BottomColdDesc> b;
		auto cV = b("/home/markus/Downloads/CSV_TestFiles_2", tokenFactory);

		auto els = std::vector<std::shared_ptr<IElement>>{};
		
		std::for_each(cV->begin(), cV->end(), [&](const auto& i)
				{ 
					std::unique_ptr<BaseVisitor> civ = std::make_unique<ConsumptionVisitor<Quantity<Volume, Pure, double>>>();
					civ = i->Accept(std::move(civ));
					auto consV = civ->template As<ConsumptionVisitor<Quantity<Volume>>>();
					els.push_back(consV());	
				});

		auto med = Init(els);
		auto readings = med();

        auto accBV = (*fbv)("Accumulation","");
		accBV = readings.Accept(std::move(accBV));

		auto accV = accBV->template As<AccumulationVisitor<Quantity<Volume>>>();
		auto d = Quantity<Volume>((*accV((int)((S::Index-1)*2),(int)(S::Index*2))).To<Quantity<Volume>>());

	 	auto sum = (*accV()).To<Quantity<Volume>>();
        std::vector<typename Base::ElementType> resQ = { std::make_shared<Quantity<Scalar>>(d / (*accV()).To<Quantity<Volume>>()) };

        return Matrix<Order,DescriptorType>(typename Base::DescriptorType({resQ.size()}),ToDataType(resQ));
	}
};

class StageBase: public XBase
{
	using Base = XBase;
protected:
	StageBase(std::shared_ptr<Factory<IToken>> fT,std::shared_ptr<Factory<IElement>> fE,std::shared_ptr<Factory<BaseVisitor>> fB, const std::string& p): XBase{fT,fE,fB}, parser{std::make_unique<StageParser>(fT,p)} {};
	std::unique_ptr<IMatrixParser<2>> parser;
private:
	const std::string fileName = "SN_Name.csv";
	typename Base::MatrixType exec() const	{	return matrix();	}
	virtual typename Base::MatrixType matrix() const = 0;
};

class Stages: public StageBase
{
	using Base = StageBase;
public:
	Stages(std::shared_ptr<Factory<IToken>> fT,std::shared_ptr<Factory<IElement>> fE,std::shared_ptr<Factory<BaseVisitor>> fB, const std::string& p): StageBase{fT,fE,fB, p} {};
private:
	virtual typename Base::MatrixType matrix() const { return (*parser)().Cols(8,9,10)[1];	}
};

class ExtraCosts: public StageBase
{
	using Base = StageBase;
public:
	ExtraCosts(std::shared_ptr<Factory<IToken>> fT,std::shared_ptr<Factory<IElement>> fE,std::shared_ptr<Factory<BaseVisitor>> fB, const std::string& p): StageBase{fT,fE,fB, p} {};
private:
	virtual typename Base::MatrixType matrix() const
	{
        auto payment = (*parser)().Cols(8,9,10).To<Quantity<Sum>>();
        std::vector<std::shared_ptr<IElement>> extras = 
			{	std::make_shared<Quantity<Sum>>(payment[1][1].To<Quantity<Sum>>()+payment[1][2].To<Quantity<Sum>>()),
				std::make_shared<Quantity<Sum>>(payment[2][1].To<Quantity<Sum>>()+payment[2][2].To<Quantity<Sum>>())
			}; 
        std::vector<std::shared_ptr<IElement>> extrasR;

        std::for_each(extras.begin(), extras.end(),[&](auto& e) { extrasR.push_back(std::make_shared<Quantity<Sum>>(e->template To<Quantity<Sum>>() * Quantity<Scalar>{12}) ); });

        return Matrix<Order,DescriptorType>(typename Base::DescriptorType({extrasR.size()}),ToDataType(extrasR));
	}
};

class Account: public XBase
{
	using Base = XBase;
public:
	Account(std::shared_ptr<Factory<IToken>> fT,std::shared_ptr<Factory<IElement>> fE,std::shared_ptr<Factory<BaseVisitor>> fB, const std::string& p): XBase{fT,fE,fB} {};
private:
	typename Base::MatrixType exec() const
	{
		using MDS2 = MatrixDescriptor<2,std::string>;
        using MS2 = Matrix<2,MDS2>;
		using TF = TypeFactory<CompositeFactory<IPredicateVisitor, Factory<IElement>>, EqualVisitor, LessVisitor>;
		
		auto parsedAccountMatrix = AccountParser(tokenFactory,"/home/markus/Downloads/CSV_TestFiles_2")();
		
		auto typeFactory = std::make_shared<TF>(elementFactory);

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

		auto all = std::make_unique<MatrixComposite<decltype(parsedAccountMatrix)>>("All");//, mcHeating.Clone());

        std::vector<FactoryUnit<std::string, std::string>> fv{{"Accumulation"}};
        for(uint i = 0; i < allFactoryUnits.size(); ++i)
                 all->Add(MatrixComposite<decltype(parsedAccountMatrix)>::Create(typeFactory,visitorFactory,std::move(allFactoryUnits[i].Name()), allFactoryUnits[i].Units(),fv));
        auto result = (*all)(parsedAccountMatrix);
		auto ms = result->Elements().To<Quantity<Sum>>();
		return result->Elements();
	}
};
