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
	typename Base::MatrixType exec(const HTMLBuilder<German>& f)
	{
		Builder<ICounter,Counter, BottomHotDesc, BottomColdDesc, MiddleHotDesc, MiddleColdDesc,TopHotDesc, TopColdDesc> b;
		auto cV = b("/home/markus/Downloads/CSV_TestFiles_2", tokenFactory);

		auto els = std::vector<std::shared_ptr<IElement>>{};
		
		std::for_each(cV->begin(), cV->end(), [&](const auto& i)
				{ 
        			auto civ = (*Base::visitorFactory)("ConsumptionVolume","");
					civ = i->Accept(std::move(civ));
					auto consV = civ->template As<ConsumptionVisitor<Quantity<Volume>>>();
					els.push_back(consV(Base::year));	
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
class Hall: public CalculatorBase<Quantity<Sum>, Hall<S>>
{
	using Base = CalculatorBase<Quantity<Sum>, Hall<S>>;
public:
	auto M() const { return parser->M().Rows(0, 1); }
	Hall(std::shared_ptr<Factory<IToken>> fT,std::shared_ptr<Factory<IElement>> fE,std::shared_ptr<Factory<BaseVisitor>> fB, const Year& y,const std::string& p): 
		Base{fE,fB,y}, path{p}, tokenFactory{fT},parser{std::make_unique<HallParser>(tokenFactory,path)} {};
	const std::string path;
	std::shared_ptr<Factory<IToken>> tokenFactory;
	std::unique_ptr<IMatrixParser<2>> parser;
private:
	const std::string fileName = "Hall.csv";
	typename Base::MatrixType exec(const HTMLBuilder<German>& f) 
	{
		auto m = (*parser)();
		std::cout<<"HALL\n"<<m<<std::endl;
		for(auto i = 1; i < m.Rows();++i)
		{
			std::unique_ptr<BaseVisitor> fc = std::make_unique<ComposedFuncVisitor<Quantity<SumPerArea>, FuncVisitor<QL,QL,Mul>,Mul>>();
		    fc = m[i].Accept(std::move(fc));
	    	auto fC = fc->template As<ComposedFuncVisitor<Quantity<SumPerArea>, FuncVisitor<QL,QL,Mul>,Mul>>();
			auto m = (*parser)(true);
			std::cout<<"M\n"<<m[i][0]<<std::endl;
			auto mf = MatrixFormatter(m.Rows(0,S::Index-1));
        	auto html = HTMLBuilder(m[i][0]()->Data()+"Hall.html","/home/markus/Downloads/CSV_TestFiles_2");
			html(mf());
	//		std::unique_ptr<BaseVisitor> fv = std::make_unique<FuncVisitor<QS,Quantity<SumPerArea>, Mul>>();
	//    	fv = m[0].Accept(std::move(fv));
	//    	auto fV = fv->template As<FuncVisitor<QS,Quantity<SumPerArea>, Mul>>();
			
			std::cout<<"HALL\n"<<Mul{Constant{QSC{12}},fC.F()}<<"="<<Mul{Constant{QSC{12}},fC.F()}()<<std::endl;
		}
		return (*parser)()[0];	}
	
	//virtual typename Base::MatrixType matrix(HTMLBuilder<German> f) const = 0;
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
	typename Base::MatrixType exec(const HTMLBuilder<German>& f) {	return matrix(f);	}
	virtual typename Base::MatrixType matrix(const HTMLBuilder<German>& f) const = 0;
};

template<typename S>
class ExtraCostItemsCalculator: public StageBase<S>
{
	using Base = StageBase<S>;
public:
	ExtraCostItemsCalculator(std::shared_ptr<Factory<IToken>> fT,std::shared_ptr<Factory<IElement>> fE,std::shared_ptr<Factory<BaseVisitor>> fB, const Year& y,const std::string& p): Base{fT,fE,fB, y, p} {};
private:
	virtual typename Base::MatrixType matrix(const HTMLBuilder<German>& f) const { return (*Base::parser)().Cols(8,9,10)[S::Index-1];	}
};

template<typename S>
class YearlyExtraCostsCalculator: public StageBase<S>
{
	using Base = StageBase<S>;
public:
	YearlyExtraCostsCalculator(std::shared_ptr<Factory<IToken>> fT,std::shared_ptr<Factory<IElement>> fE,std::shared_ptr<Factory<BaseVisitor>> fB, const Year& y,const std::string& p): Base{fT,fE,fB, y,p} {};
private:
	virtual typename Base::MatrixType matrix(const HTMLBuilder<German>& f) const
	{
		auto m = (*(Base::parser))();
		auto mf1 = MatrixFormatter(m[S::Index-1]);
        auto html = HTMLBuilder<German>(std::to_string(S::Index)+"_S.html","/home/markus/Downloads/CSV_TestFiles_2");
		html(mf1());
		f(mf1());
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
	typename Base::MatrixType exec(const HTMLBuilder<German>& f) 
	{
		using MDS2 = MatrixDescriptor<2,std::string>;
        using MS2 = Matrix<2,MDS2>;
		
		using TF = TypeFactory<CompositeFactory<IPredicateVisitor, Factory<IElement>>, EqualVisitor, LessVisitor>;
		auto typeFactory = std::make_shared<TF>(elementFactory);
		
		auto parsedAccountMatrix = (*parser)();

		//std::cout<<parsedAccountMatrix<<std::endl;

		std::vector<FactoryUnitContainer<FactoryUnitContainer<FactoryUnit<std::string,FactoryUnit<std::string, std::string>>>>> allFactoryUnits = 
        {
            {"Waste",
                {
                
                    {"Deduction",{{EqualVisitor::Identifier, { IBAN::Identifier, "DE44600501010008017284"}}, {EqualVisitor::Identifier, { Year::Identifier, Base::year.ToString()}}}} // Waste
                }
            }, 
            {"Heating",
                {
                    {"Deduction",{{EqualVisitor::Identifier, { Entry::Identifier, "Abschlagsforderung"}}, {EqualVisitor::Identifier, { Entry::Identifier, "701006843905"}}, {EqualVisitor::Identifier, { IBAN::Identifier, "DE56600501017402051588"}}, {EqualVisitor::Identifier, { Year::Identifier, Base::year.ToString()}}}},
                    {"Deduction",{{EqualVisitor::Identifier, { Entry::Identifier, "Abschlagsforderung"}}, {EqualVisitor::Identifier, { IBAN::Identifier, "DE68600501010002057075"}}, {EqualVisitor::Identifier, { Year::Identifier, Base::year.ToString()}}}},
                    {"Invoice",{{EqualVisitor::Identifier, { Entry::Identifier, "Rechnung"}}, {EqualVisitor::Identifier, { IBAN::Identifier, "DE56600501017402051588"}}, {EqualVisitor::Identifier, { Year::Identifier, Base::year.Next().ToString()}}}},
                    {"Rechnung",{{EqualVisitor::Identifier, { Entry::Identifier, "Rechnung"}}, {EqualVisitor::Identifier, { IBAN::Identifier, "DE68600501010002057075"}}, {EqualVisitor::Identifier, { Year::Identifier, Base::year.Next().ToString()}}}},
                    //{"Maintenance",{{EqualVisitor::Identifier, { IBAN::Identifier, "DE73660623660009271112"}}, {EqualVisitor::Identifier, { Year::Identifier, "2022"}}}},
                }
            },
            {"BuildingInsurance",{
                    {"Invoice",{{EqualVisitor::Identifier, { IBAN::Identifier, "DE97500500000003200029"}}, {EqualVisitor::Identifier, { Year::Identifier, Base::year.ToString()}}}} // Insurance
                }
            },
            {"Cleaning",{
                    {"Alles Proper",{{EqualVisitor::Identifier, { IBAN::Identifier, "DE05100110012620778704"}}, {EqualVisitor::Identifier, { Year::Identifier, Base::year.ToString()}}}},
                    {"Jansen",{{EqualVisitor::Identifier, { IBAN::Identifier, "DE08548500101700257437"}}, {EqualVisitor::Identifier, { Year::Identifier, Base::year.ToString()}}}},
                    {"Jansen",{{EqualVisitor::Identifier, { IBAN::Identifier, "DE08548500101700257437"}}, {EqualVisitor::Identifier, { Month::Identifier, "1"}}}},
                    {"Rastaetter",{{EqualVisitor::Identifier, { IBAN::Identifier, "DE79660623660000101303"}}, {EqualVisitor::Identifier, { Year::Identifier, Base::year.ToString()}}}},
                }
            },
            {"PropertyTax",{
                    {"Deduction",{{EqualVisitor::Identifier, { IBAN::Identifier, "DE12660623660000005703"}}, {EqualVisitor::Identifier, { Year::Identifier, Base::year.ToString()}}, {EqualVisitor::Identifier, { Entry::Identifier, "501000000891/Grundsteuer"}}}} //Grundsteuer
                }
            },
            {"Sewage",{
                    {"Deduction",{{EqualVisitor::Identifier, { IBAN::Identifier, "DE12660623660000005703"}}, {EqualVisitor::Identifier, { Year::Identifier, Base::year.ToString()}}, {EqualVisitor::Identifier, { Entry::Identifier, "Abschlag/Abwasser"}}}}, //Abwasser
                    {"Invoice",{{EqualVisitor::Identifier, { IBAN::Identifier, "DE12660623660000005703"}}, {EqualVisitor::Identifier, { Year::Identifier, Base::year.Next().ToString()}}, {EqualVisitor::Identifier, { Entry::Identifier, "Rechnung/Abwasser"}}}} //Abwasser
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

//		(*result)(*f);

//		std::cout<<*result<<std::endl;
//		std::cout<<"ELEMENTS: "<<result->Elements()<<std::endl;

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
	virtual typename Base::MatrixType matrix(const HTMLBuilder<German>& f) const
	{ 
		auto stageMatrix = (*Base::parser)().Cols(2,3,4,5,6,7).template To<Quantity<Scalar>>();
		using AllStages = std::tuple<Bottom, Middle, Top>;
		stageMatrix = process<0,AllStages>(stageMatrix,Base::tokenFactory,Base::elementFactory,Base::visitorFactory, Base::path,f);
        auto costs = calcCosts<0,AllStages>(stageMatrix,Base::tokenFactory,Base::elementFactory,Base::visitorFactory, Base::path,f).Rows(S::Index-1);
		return (*Base::parser)()[S::Index-1];	
	}
	template<size_t N, typename Tup>
    auto process(auto& stageMatrix, std::shared_ptr<Factory<IToken>> fT,std::shared_ptr<Factory<IElement>> fE,std::shared_ptr<Factory<BaseVisitor>> fB, const std::string& p, const HTMLBuilder<German>& f) const 
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
    auto calcCosts(auto stageMatrix, std::shared_ptr<Factory<IToken>> tokenFactory,std::shared_ptr<Factory<IElement>> elementFactory,std::shared_ptr<Factory<BaseVisitor>> visitorFactory, const std::string& path, const HTMLBuilder<German>& f) const
    {
        auto account = AccountCalculator{tokenFactory,elementFactory,visitorFactory, Base::year, path}; 
        stageMatrix = process<0,Tup>(stageMatrix,tokenFactory,elementFactory,visitorFactory, path, f);

		auto mf = MatrixFormatter((*Base::parser)(true).Rows(0,S::Index));
        auto html = HTMLBuilder(std::to_string(S::Index)+".html","/home/markus/Downloads/CSV_TestFiles_2");
		html(Date::Today());
		html(mf());

        auto sumMatrix = account(html).template To<Quantity<Sum>>();  
        auto csum = stageMatrix.ColSum()();
        auto stagesDiv = (stageMatrix / csum());

		auto result = stagesDiv[S::Index-1] * sumMatrix;
		auto res = result().template To<Quantity<Sum>>();
		std::vector<std::vector<std::string>> v;
		for(size_t i = 0; i < 6; ++i)
		{
			std::vector<std::string> vr;
			expand(vr, stageMatrix[S::Index-1][i], csum[i], stagesDiv[S::Index-1][i], stagesDiv[S::Index-1][i](), sumMatrix[i], result[i][i], res[i][i]);
			v.push_back(vr);
		}

		append(Init(v)(),html);
		append(this->M(),html);

//		for(size_t i = 0; i < 6; ++i)
//			*f<<stageMatrix[S::Index-1][i]<<"\t"<<csum[i]<<"\t"<<stagesDiv[S::Index-1][i]<<"\t"<<stagesDiv[S::Index-1][i]()<<"\t"<<result[i][i]<<"\t"<<res[i][i]<<std::endl;
        return stagesDiv * sumMatrix;                                                                                                       
    }

	template<typename T, typename... R>
	auto expand(std::vector<std::string>& v, const T& val, const R... r) const
	{
		sstream(v,val);
		expand(v,r...);
	}

	template<typename T>
	auto expand(std::vector<std::string>& v, const T& val) const {	sstream(v,val);	}
	
	auto sstream(std::vector<std::string>& v, const auto& val) const
	{
		std::stringstream ss;
		ss<<val;
		v.push_back(ss.str());
	}
	
	auto append(const auto& m, const HTMLBuilder<German>& out) const
	{
		auto mf = MatrixFormatter(m);
		out(mf());
	}
};
