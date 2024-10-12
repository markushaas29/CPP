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

#pragma once

class AccountCalculator: public CalculatorBase<Quantity<Sum>, AccountCalculator>
{
	using Base = CalculatorBase<Quantity<Sum>, AccountCalculator>;
public:
	AccountCalculator(std::shared_ptr<Factory<IToken>> fT,std::shared_ptr<Factory<IElement>> fE,std::shared_ptr<Factory<BaseVisitor>> fB, const std::string& p): Base{fE,fB}, tokens{fT}, path{p} {};
private:
	std::string path;
	std::shared_ptr<Factory<IToken>> tokens;
	std::unique_ptr<IResult<Quantity<Unit<1>>, Matrix<2, MatrixDescriptor<2,std::shared_ptr<IElement>>>>, std::default_delete<IResult<Quantity<Unit<1>>, Matrix<2, MatrixDescriptor<2, std::shared_ptr<IElement>>>>>> result;
	typename Base::MatrixType exec(const Year& y, const HtmlBuilder<German>& f) 
	{
		using MDS2 = MatrixDescriptor<2,std::string>;
        using MS2 = Matrix<2,MDS2>;
		
		using TF = TypeFactory<CompositeFactory<IPredicateVisitor, Factory<IElement>>, EqualVisitor, LessVisitor>;
		auto typeFactory = std::make_shared<TF>(elementFactory);
		
		auto parsedAccountMatrix = AccountParser::Instance(tokens,path)();

		//std::cout<<parsedAccountMatrix<<std::endl;

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
                    {"Invoice",{{EqualVisitor::Identifier, { Entry::Identifier, "Rechnung"}}, {EqualVisitor::Identifier, { IBAN::Identifier, "DE56600501017402051588"}}, {EqualVisitor::Identifier, { Year::Identifier, y.Next().ToString()}}}},
                    {"Rechnung",{{EqualVisitor::Identifier, { Entry::Identifier, "Rechnung"}}, {EqualVisitor::Identifier, { IBAN::Identifier, "DE68600501010002057075"}}, {EqualVisitor::Identifier, { Year::Identifier, y.Next().ToString()}}}},
                    //{"Maintenance",{{EqualVisitor::Identifier, { IBAN::Identifier, "DE73660623660009271112"}}, {EqualVisitor::Identifier, { Year::Identifier, "2022"}}}},
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
                    {"Jansen",{{EqualVisitor::Identifier, { IBAN::Identifier, "DE08548500101700257437"}}, {EqualVisitor::Identifier, { Month::Identifier, "1"}}}},
                    {"Rastaetter",{{EqualVisitor::Identifier, { IBAN::Identifier, "DE79660623660000101303"}}, {EqualVisitor::Identifier, { Year::Identifier, y.ToString()}}}},
                }
            },
            {"PropertyTax",{
                    {"Deduction",{{EqualVisitor::Identifier, { IBAN::Identifier, "DE12660623660000005703"}}, {EqualVisitor::Identifier, { Year::Identifier, y.ToString()}}, {EqualVisitor::Identifier, { Entry::Identifier, "501000000891/Grundsteuer"}}}} //Grundsteuer
                }
            },
            {"Sewage",{
                    {"Deduction",{{EqualVisitor::Identifier, { IBAN::Identifier, "DE12660623660000005703"}}, {EqualVisitor::Identifier, { Year::Identifier, y.ToString()}}, {EqualVisitor::Identifier, { Entry::Identifier, "Abschlag/Abwasser"}}}}, //Abwasser
                    {"Invoice",{{EqualVisitor::Identifier, { IBAN::Identifier, "DE12660623660000005703"}}, {EqualVisitor::Identifier, { Year::Identifier, y.Next().ToString()}}, {EqualVisitor::Identifier, { Entry::Identifier, "Rechnung/Abwasser"}}}} //Abwasser
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

		auto mf = MatrixFormatter(result->M());  
		auto hb = HtmlBuilder<German>("Account_"+ y.ToString() +".html", "/home/markus/Downloads/CSV_TestFiles_2");
        hb(mf());
		hb();


		auto m = result->Elements().Elements();
		std::cout<<"Acc\n"<<result->Elements()<<std::endl;

		return Matrix<2, MatrixDescriptor<2,std::shared_ptr<IElement>>>(MatrixDescriptor<2,std::shared_ptr<IElement>>({1,m.size()}), m);
		//return result->Elements();
	}

};
