#include <iostream>
#include <fstream>
#include <sstream>
#include <cassert> 
#include <vector> 
#include <memory> 
#include "IAccountQuery.hpp"
#include "../Matrix/Matrix.hpp"
#include "../Matrix/MatrixReader.hpp"
#include "../Matrix/MatrixDescriptor.hpp"
#include "../Matrix/MatrixComposition.hpp"
#include "../Matrix/M3.hpp"
#include "../Matrix/MatrixParsers.hpp"
#include "../Matrix/MatrixFormatter.hpp"
#include "../Builder/Builder.hpp"
#include "../HTMLBuilder/HTMLBuilder.hpp"
#include "../Counter/ICounter.hpp"
#include "../ObjectFactory/Factory.hpp"
#include "../Common/DateTimes.hpp"
#include "../CSV/Elements.hpp"
#include "../CSV/NumericStyle.hpp"
#include "../CSV/Matcher.hpp"
#include "../Quantity/Quantity.hpp"
#include "../Functional/Functional.hpp"
#include "../Common/DateTimes.hpp"
#include "../Visitor/CollectorVisitor.hpp"
#include "../Invoice/InvoiceCalculatorBase.hpp"
#include "../Query/Query.hpp"

#pragma once

class AccountCalculatorBase: public CalculatorBase<Quantity<Sum>, AccountCalculatorBase>
{
	using Base = CalculatorBase<Quantity<Sum>, AccountCalculatorBase>;
public:
	AccountCalculatorBase(std::shared_ptr<Factory<IToken>> fT,std::shared_ptr<Factory<IElement>> fE,std::shared_ptr<Factory<BaseVisitor>> fB, const std::string& p): Base{fE,fB}, tokens{fT}, path{p} {};
	auto Get(const Year& y, const HtmlBuilder<German>& f, const std::vector<FactoryUnitContainer<FactoryUnitContainer<FactoryUnit<std::string,FactoryUnit<std::string, std::string>>>>>& allFactoryUnits, const std::string& n = "Test_") 
	{
		return exec(y,f,allFactoryUnits,n);
	}
protected:
	typename Base::MatrixType exec(const Year& y, const HtmlBuilder<German>& f, const std::vector<FactoryUnitContainer<FactoryUnitContainer<FactoryUnit<std::string,FactoryUnit<std::string, std::string>>>>>& allFactoryUnits, const std::string& n) const
	{
		using MDS2 = MatrixDescriptor<2,std::string>;
        using MS2 = Matrix<2,MDS2>;
		
		using TF = TypeFactory<CompositeFactory<IPredicateVisitor, Factory<IElement>>, EqualVisitor, LessVisitor>;
		auto typeFactory = std::make_shared<TF>(elementFactory);
		
		auto parsedAccountMatrix = AccountParser::Instance(tokens,path)();

		auto fu = std::make_shared<std::ofstream>("/home/markus/Downloads/CSV_TestFiles_2/FactoryUnits.txt");
		for(auto u : allFactoryUnits)
			(*fu)<<u;
		fu->close();

		auto all = std::make_unique<MatrixComposite<decltype(parsedAccountMatrix)>>("All");

        std::vector<FactoryUnit<std::string, std::string>> fv{{"AccumulationSum"}};
        for(uint i = 0; i < allFactoryUnits.size(); ++i)
                 all->Add(MatrixComposite<decltype(parsedAccountMatrix)>::Create(typeFactory,visitorFactory,std::move(allFactoryUnits[i].Name()), allFactoryUnits[i].Units(),fv));
        std::unique_ptr<IResult<Quantity<Unit<1>>, Matrix<2, MatrixDescriptor<2,std::shared_ptr<IElement>>>>, std::default_delete<IResult<Quantity<Unit<1>>, Matrix<2, MatrixDescriptor<2, std::shared_ptr<IElement>>>>>> result = (*all)(parsedAccountMatrix);

		auto hb = HtmlBuilder<German>(n + y.ToString() +".html", "/home/markus/Dokumente/cpp/CSV_Files");
		auto heads = std::make_unique<std::vector<std::unique_ptr<IHtmlElement>>>();
		heads->push_back(std::make_unique<LinkElement>("AccountStyle"));
		hb.Add(std::make_unique<HtmlElements<Head>>(std::move(heads)));
        hb.Add(result->Html());
		hb();

		auto names = result->Names().Elements();
		auto elements = result->Elements().Elements();
		elements.insert(std::end(elements), std::begin(names), std::end(names));

		return Matrix<2, MatrixDescriptor<2,std::shared_ptr<IElement>>>(MatrixDescriptor<2,std::shared_ptr<IElement>>({2,names.size()}), elements);
	}
private:
	std::string path;
	std::shared_ptr<Factory<IToken>> tokens;
	std::unique_ptr<IResult<Quantity<Unit<1>>, Matrix<2, MatrixDescriptor<2,std::shared_ptr<IElement>>>>, std::default_delete<IResult<Quantity<Unit<1>>, Matrix<2, MatrixDescriptor<2, std::shared_ptr<IElement>>>>>> result;
	virtual Matrix<2,MatrixDescriptor<2,std::shared_ptr<IElement>>> getTokens(const Year& y, const HtmlBuilder<German>& f, std::unique_ptr<IAccountQuery> = std::make_unique<ExtraCostQuery>()) const = 0;
	virtual typename Base::MatrixType exec(const Year& y, const HtmlBuilder<German>& f) { return getTokens(y,f); };
};


class IAccount
{
public:
	auto GetT(std::unique_ptr<IAccountQuery> q, const HtmlBuilder<German>& f) const { get(std::move(q),f); }
private:
	virtual Matrix<2,MatrixDescriptor<2,std::shared_ptr<IElement>>> get(std::unique_ptr<IAccountQuery> q, const HtmlBuilder<German>& f) const = 0;
};

class AccountCalculator: public AccountCalculatorBase, public IAccount
{
	using Base = AccountCalculatorBase;
public:
	AccountCalculator(std::shared_ptr<Factory<IToken>> fT,std::shared_ptr<Factory<IElement>> fE,std::shared_ptr<Factory<BaseVisitor>> fB, const std::string& p): Base{fT,fE,fB,p} {};
private:
	virtual Matrix<2,MatrixDescriptor<2,std::shared_ptr<IElement>>> get(std::unique_ptr<IAccountQuery> q, const HtmlBuilder<German>& f) const { return getTokens(Year{2025},f,std::move(q)); };
	virtual Matrix<2,MatrixDescriptor<2,std::shared_ptr<IElement>>> getTokens(const Year& y, const HtmlBuilder<German>& f, std::unique_ptr<IAccountQuery> = std::make_unique<ExtraCostQuery>()) const
	{
		std::vector<FactoryUnitContainer<FactoryUnitContainer<FactoryUnit<std::string,FactoryUnit<std::string, std::string>>>>> allFactoryUnits = 
        {
            {"Waste",
                {
                
                    {"Deduction",{{EqualVisitor::Identifier, { IBAN::Identifier, "DE44600501010008017284"}}, {EqualVisitor::Identifier, { Year::Identifier, y.ToString()}}}} // Waste
                }
            }, 
            {"Heating",
                {
                    {"Deduction",{{EqualVisitor::Identifier, { Entry::Identifier, "Abschlagsforderung"}}, {EqualVisitor::Identifier, { Entry::Identifier, "701006843905"}}, {EqualVisitor::Identifier, { IBAN::Identifier, "DE56600501017402051588"}}, {EqualVisitor::Identifier, { Year::Identifier, y.ToString()}}}},
                    {"Deduction",{{EqualVisitor::Identifier, { Entry::Identifier, "Abschlagsforderung"}}, {EqualVisitor::Identifier, { IBAN::Identifier, "DE68600501010002057075"}}, {EqualVisitor::Identifier, { Year::Identifier, y.ToString()}}}},
                    //{"Deduction",{{EqualVisitor::Identifier, { IBAN::Identifier, "DE92600501010004021281"}}, {EqualVisitor::Identifier, { Year::Identifier, y.ToString()}}}},
                    {"Deduction",{{EqualVisitor::Identifier, { Entry::Identifier, "Dettenheim/AB/GAS/"}}, {EqualVisitor::Identifier, { IBAN::Identifier, "DE92600501010004021281"}}, {EqualVisitor::Identifier, { Year::Identifier, y.ToString()}}}},
                    {"Invoice",{{EqualVisitor::Identifier, { Entry::Identifier, "Rechnung"}}, {EqualVisitor::Identifier, { IBAN::Identifier, "DE56600501017402051588"}}, {EqualVisitor::Identifier, { Month::Identifier, Jan.ToString()}}, {EqualVisitor::Identifier, { Year::Identifier, y.Next().ToString()}}}},
                    {"Rechnung",{{EqualVisitor::Identifier, { Entry::Identifier, "Rechnung"}}, {EqualVisitor::Identifier, { IBAN::Identifier, "DE68600501010002057075"}}, {EqualVisitor::Identifier, { Year::Identifier, y.Next().ToString()}}}},
                    {"Rechnung",{{EqualVisitor::Identifier, { Entry::Identifier, "Dettenheim/RE/GAS/"}}, {EqualVisitor::Identifier, { IBAN::Identifier, "DE92600501010004021281"}}, {EqualVisitor::Identifier, { Year::Identifier, y.Next().ToString()}}}},
                    {"Maintenance",{{EqualVisitor::Identifier, { IBAN::Identifier, "DE73660623660009271112"}}, {EqualVisitor::Identifier, { Year::Identifier, y.ToString()}}}},
                    {"Maintenance",{{EqualVisitor::Identifier, { IBAN::Identifier, "DE82660501011021592702"}}, {EqualVisitor::Identifier, { Year::Identifier, y.ToString()}}}},
                }
            },
            {"BuildingInsurance",{
                    {"Invoice",{{EqualVisitor::Identifier, { IBAN::Identifier, "DE97500500000003200029"}}, {EqualVisitor::Identifier, { Year::Identifier, y.ToString()}}}} // Insurance
                }
            },
            {"Cleaning",{
                    {"Alles Proper",{{EqualVisitor::Identifier, { IBAN::Identifier, "DE05100110012620778704"}}, {EqualVisitor::Identifier, { Year::Identifier, y.ToString()}}}},
                    {"Jansen",{{EqualVisitor::Identifier, { IBAN::Identifier, "DE08548500101700257437"}}, {EqualVisitor::Identifier, { Year::Identifier, y.ToString()}}}},
                    {"Pfälzer Immobilien Betreuungs GmbH",{{EqualVisitor::Identifier, { IBAN::Identifier, "DE92120300001058229483"}}, {EqualVisitor::Identifier, { Year::Identifier, y.ToString()}}}},
                    {"Rastaetter",{{EqualVisitor::Identifier, { IBAN::Identifier, "DE79660623660000101303"}}, {EqualVisitor::Identifier, { Year::Identifier, y.ToString()}}}},
                }
            },
            {"PropertyTax",{
                    {"Deduction",{{EqualVisitor::Identifier, { IBAN::Identifier, "DE12660623660000005703"}}, {EqualVisitor::Identifier, { Year::Identifier, y.ToString()}}, {EqualVisitor::Identifier, { Entry::Identifier, "501000000891/Grundsteuer"}}}} //Grundsteuer
                }
            },
            {"Sewage",{
                    {"Deduction",{{EqualVisitor::Identifier, { IBAN::Identifier, "DE12660623660000005703"}}, {EqualVisitor::Identifier, { Year::Identifier, y.ToString()}}, {EqualVisitor::Identifier, { Entry::Identifier, "588880002829/Schmutzwasser"}}}}, //Abwasser
                    {"Deduction",{{EqualVisitor::Identifier, { IBAN::Identifier, "DE12660623660000005703"}}, {EqualVisitor::Identifier, { Year::Identifier, y.ToString()}}, {EqualVisitor::Identifier, { Entry::Identifier, "Abschlag/Abwasser"}}}}, //Abwasser
                    {"Invoice",{{EqualVisitor::Identifier, { IBAN::Identifier, "DE12660623660000005703"}}, {EqualVisitor::Identifier, { Year::Identifier, y.Next().ToString()}}, {EqualVisitor::Identifier, { Entry::Identifier, "Rechnung/Abwasser"}}}}, //Abwasser
                    {"Invoice",{{EqualVisitor::Identifier, { IBAN::Identifier, "DE12660623660000005703"}}, {EqualVisitor::Identifier, { Year::Identifier, y.Next().ToString()}}, {EqualVisitor::Identifier, { Entry::Identifier, "588880002829/Schmutzwasse Rechnung 2025"}}}}, //Abwasser
                    {"Invoice",{{EqualVisitor::Identifier, { IBAN::Identifier, "DE34660501010100000058"}}, {EqualVisitor::Identifier, { Year::Identifier, y.Next().ToString()}}, {EqualVisitor::Identifier, { Entry::Identifier, "/Schmutzwasser 2025 RUCHENSTR. 14/Wasser 2025 RUCHENSTR. 14 EREF: 110900003850"}}}}, //Abwasser
                    //{"Invoice",{{EqualVisitor::Identifier, { IBAN::Identifier, "DE12660623660000005703"}}, {EqualVisitor::Identifier, { Year::Identifier, y.Next().ToString()}}, {EqualVisitor::Identifier, { Entry::Identifier, "/Schmutzwasser 2025 RUCHENSTR. 14/Wasser 2025 RUCHENSTR. 14 EREF: 110900003850"}}}} //Abwasser
                }
            }
        };
		
		return Base::exec(y,f,allFactoryUnits,"Account_");
	}
};

class HallCurrentCalculator: public AccountCalculatorBase
{
	using Base = AccountCalculatorBase;
public:
	HallCurrentCalculator(std::shared_ptr<Factory<IToken>> fT,std::shared_ptr<Factory<IElement>> fE,std::shared_ptr<Factory<BaseVisitor>> fB, const std::string& p): Base{fT,fE,fB,p} {};
private:
	virtual Matrix<2,MatrixDescriptor<2,std::shared_ptr<IElement>>> getTokens(const Year& y, const HtmlBuilder<German>& f, std::unique_ptr<IAccountQuery> = std::make_unique<ExtraCostQuery>()) const
	{
		std::vector<FactoryUnitContainer<FactoryUnitContainer<FactoryUnit<std::string,FactoryUnit<std::string, std::string>>>>> fUnits = 
	    {
	        {"Energy",
	            {
	            
	                {"Deduction",{{EqualVisitor::Identifier, { IBAN::Identifier, "DE56600501017402051588"}},{EqualVisitor::Identifier, { Entry::Identifier, "701033135722"}}, {EqualVisitor::Identifier, { Year::Identifier, y.ToString()}}}} // Waste
	            }
	        }, 
	    };
				
		return Base::exec(y,f,fUnits,"HallCurrent_");
	}
};
