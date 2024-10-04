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
#include "../Invoice/AccountCalculator.hpp"

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
	typename Base::MatrixType exec(const HtmlBuilder<German>& f, const Year& y)
	{
		Builder<ICounter,Counter, BottomHotDesc, BottomColdDesc, MiddleHotDesc, MiddleColdDesc,TopHotDesc, TopColdDesc> b;
		auto cV = b("/home/markus/Downloads/CSV_TestFiles_2", tokenFactory);

		auto els = std::vector<std::shared_ptr<IElement>>{};
		
		std::for_each(cV->begin(), cV->end(), [&](const auto& i)
				{ 
        			auto civ = (*Base::visitorFactory)("ConsumptionVolume","");
					civ = i->Accept(std::move(civ));
					auto consV = civ->template As<ConsumptionVisitor<Quantity<Volume>>>();
					els.push_back(consV(y));	
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
	Hall(std::shared_ptr<Factory<IToken>> fT,std::shared_ptr<Factory<IElement>> fE,std::shared_ptr<Factory<BaseVisitor>> fB, const std::string& p): 
		Base{fE,fB}, path{p}, tokenFactory{fT},parser{std::make_unique<HallParser>(tokenFactory,path)} {};
	const std::string path;
	std::shared_ptr<Factory<IToken>> tokenFactory;
	std::unique_ptr<IMatrixParser<2>> parser;
private:
	const std::string fileName = "Hall.csv";
	typename Base::MatrixType exec(const HtmlBuilder<German>& f, const Year& y) 
	{
		auto m = (*parser)();
		std::cout<<"HALL\n"<<m<<std::endl;
		for(auto i = 1; i < m.Rows();++i)
		{
			std::unique_ptr<BaseVisitor> fc = std::make_unique<ComposedFuncVisitor<Quantity<SumPerArea>, FuncVisitor<QL,QL,Mul>,Mul>>();
		    fc = m[i].Accept(std::move(fc));
	    	auto fC = fc->template As<ComposedFuncVisitor<Quantity<SumPerArea>, FuncVisitor<QL,QL,Mul>,Mul>>();
			auto m = (*parser)(true);
		  	auto mfPre = MatrixFormatter(m.Cols(std::string("Pre"),std::string("Name"),std::string("Street"),std::string("Streetnumber"),std::string("Town"),std::string("Postcode"))[i].Transform<2>(3,2));
		  	auto mf = MatrixFormatter(m.Rows(0,S::Index-1));
    		auto html = HtmlBuilder(m[i][0]()->Data()+"Hall.html","/home/markus/Downloads/CSV_TestFiles_2");
			html(mfPre());
		  	html(mf());
			std::unique_ptr<BaseVisitor> fv = std::make_unique<FuncVisitor<QS,Quantity<SumPerArea>, Mul>>();
		    fv = m[0].Accept(std::move(fv));
		    auto fV = fv->template As<FuncVisitor<QS,Quantity<SumPerArea>, Mul>>();
			std::cout<<"HALL\n"<<Mul{Constant{QSC{12}},fC.F()}<<"="<<Mul{Constant{QSC{12}},fC.F()}()<<std::endl;
		}
		return (*parser)()[0];	}
};

template<typename S>
class StageBase: public CalculatorBase<Quantity<Sum>, StageBase<S>>
{
	using Base = CalculatorBase<Quantity<Sum>, StageBase<S>>;
public:
	auto M() const { return parser->M().Rows(0, S::Index); }
protected:
	StageBase(std::shared_ptr<ICalculator<Quantity<Sum>>> acc, std::shared_ptr<Factory<IToken>> fT,std::shared_ptr<Factory<IElement>> fE,std::shared_ptr<Factory<BaseVisitor>> fB, const std::string& p): 
		Base{fE,fB}, account{acc}, path{p}, tokenFactory{fT},parser{std::make_unique<StageParser>(tokenFactory,path)} {};
	const std::string path;
	std::shared_ptr<Factory<IToken>> tokenFactory;
	std::unique_ptr<IMatrixParser<2>> parser;
	std::shared_ptr<ICalculator<Quantity<Sum>>> account;
private:
	const std::string fileName = "SN_Name.csv";
	typename Base::MatrixType exec(const HtmlBuilder<German>& f, const Year& y) {	return matrix(f,y);	}
	virtual typename Base::MatrixType matrix(const HtmlBuilder<German>& f, const Year& y) const = 0;
};

template<typename S>
class ProportionCalculator: public StageBase<S>
{
	using Base = StageBase<S>;
public:
	ProportionCalculator(std::shared_ptr<ICalculator<Quantity<Sum>>> acc, std::shared_ptr<Factory<IToken>> fT,std::shared_ptr<Factory<IElement>> fE,std::shared_ptr<Factory<BaseVisitor>> fB, const std::string& p): 
		Base{acc, fT,fE,fB, p}, properties((*Base::parser)(true).Rows(0,S::Index)), advancePayment{(properties[1][9].template As<Quantity<Sum>>()+properties[1][10].template As<Quantity<Sum>>()) * Quantity<Scalar>{12}} {};
	auto AdvancePayment() { return advancePayment; }
	auto Properties() { return properties; }
	auto AdvanceItems() { return properties.Cols(8,9,10); }
	auto Result(const Year& y) { return this->Value(y) + advancePayment; }
private:
	Matrix<2, MatrixDescriptor<2,typename Base::ElementType>> properties;
	Quantity<Sum> advancePayment;
	virtual typename Base::MatrixType matrix(const HtmlBuilder<German>& f, const Year& y) const
	{
		auto stageMatrix = (*Base::parser)().Cols(2,3,4,5,6,7).template To<Quantity<Scalar>>();
		using AllStages = std::tuple<Bottom, Middle, Top>;
		stageMatrix = process<0,AllStages>(stageMatrix,Base::tokenFactory,Base::elementFactory,Base::visitorFactory, Base::path,f,y);
        auto costs = calcCosts<0,AllStages>(stageMatrix,Base::tokenFactory,Base::elementFactory,Base::visitorFactory, Base::path,f,y);
		return costs;	
	}
	template<size_t N, typename Tup>
    auto process(auto& stageMatrix, std::shared_ptr<Factory<IToken>> fT,std::shared_ptr<Factory<IElement>> fE,std::shared_ptr<Factory<BaseVisitor>> fB, const std::string& p, const HtmlBuilder<German>& f, const Year& y) const 
    {
        if constexpr (std::tuple_size<Tup>()==N)
            return stageMatrix;
        else
        {
            using Type = std::tuple_element_t<N,Tup>;
            auto readings = Readings<Type>{fT,fE,fB, p};
            stageMatrix = stageMatrix.Set(readings(f,y)[0].template As<Quantity<Scalar>>(),Type::Index-1,((int)stageMatrix.Cols()-1));
            return process<N+1,Tup>(stageMatrix,fT,fE,fB,p,f,y);
        }
    }
    
    template<size_t N, typename Tup>
    auto calcCosts(auto stageMatrix, std::shared_ptr<Factory<IToken>> tokenFactory,std::shared_ptr<Factory<IElement>> elementFactory,std::shared_ptr<Factory<BaseVisitor>> visitorFactory, const std::string& path, const HtmlBuilder<German>& f, const Year& y) const
    {
        stageMatrix = process<0,Tup>(stageMatrix,tokenFactory,elementFactory,visitorFactory, path, f, y);

		auto stageQuantities = (*Base::parser)(true).Rows(0,S::Index);
		auto stageQT = stageQuantities^-1;
		auto mf = MatrixFormatter(stageQT);
        auto html = HtmlBuilder(std::to_string(S::Index)+"_"+y.ToString()+".html","/home/markus/Downloads/CSV_TestFiles_2");
//		html(Date::Today());

		auto outs = std::make_unique<std::vector<std::unique_ptr<IHtmlElement>>>();
		auto classCss = std::make_unique<StyleElement>();
		classCss->Add(std::make_unique<ClassCss<Border,Style<Padding,Px<14>>>>());
		outs->push_back(std::move(classCss));
		auto div0 = std::make_unique<HtmlElements<DivTag>>("Div0",std::make_unique<Css<Style<GridArea,AreaNum<1>>,Style<BackgroundColor,RGB<255,1,1>>>>());
		div0->Add(Date::Today().Html());
		outs->push_back(std::move(div0));

		auto div1 = std::make_unique<HtmlElements<DivTag>>("Div1",std::make_unique<Css<Style<GridArea,AreaNum<2>>,Style<BackgroundColor,Hex<"f9f9f9">>>>());
		div1->Add(mf.Html(std::make_unique<HtmlElement<Caption, Header>>(Header("Caption"))));
		outs->push_back(std::move(div1));

        auto sumMatrix = (*Base::account)(html, y).template To<Quantity<Sum>>();  
        auto csum = stageMatrix.ColSum()();
        auto stagesDiv = (stageMatrix / csum());

		auto result = stagesDiv[S::Index-1] * sumMatrix;
		auto res = result().template To<Quantity<Sum>>();
		std::vector<std::vector<std::shared_ptr<IElement>>> vp;

		for(size_t i = 0; i < 6; ++i)
		{
			std::vector<std::shared_ptr<IElement>> vpr;
			vpr.push_back(std::make_shared<Quantity<Scalar,Pure,double>>(asString(stageMatrix[S::Index-1][i])));
			vpr.push_back(csum[i].Get().Clone());
			vpr.push_back(std::make_shared<Entry>(asString(stagesDiv[S::Index-1][i])));
			vpr.push_back(stagesDiv[S::Index-1][i].Get().Clone());
			vpr.push_back(sumMatrix[i].Get().template To<Quantity<Sum>>().Clone());
			vpr.push_back(std::make_shared<Entry>(asString(result[i][i])));
			vpr.push_back(res[i][i].Get().template To<Quantity<Sum>>().Clone());
			vp.push_back(vpr);
		}

		auto resultElements = Init(vp)();
		auto mfE = MatrixFormatter(resultElements);
		auto div2 = std::make_unique<HtmlElements<DivTag>>("Div1",std::make_unique<Css<Style<GridArea,AreaNum<3>>,Style<BackgroundColor,Hex<"f9f9f9">>>>());
		div2->Add(mfE.Html());
		
		auto sum = resultElements.Col(6).template To<Quantity<Sum>>().ColSum();
		auto extraCosts = stageQuantities[1].Rows(9,10).template To<Quantity<Sum>>();
  		auto yearCosts = (extraCosts) * Quantity<Scalar>{12};
		auto heatingPayment = yearCosts[0];
		auto advancedPayment = yearCosts[1];

		auto payment = yearCosts.ColSum();
		auto resultSum = Constant(Quantity<Sum>{sum()}) + Constant(Quantity<Sum>{payment()});
		std::vector<std::vector<std::shared_ptr<IElement>>> costs = 
		{
			{std::make_shared<Header>("ABC"),std::make_shared<Entry>(asString(sum)), std::make_shared<Quantity<Sum>>(sum())},
			{stageQuantities[0][9]()->Clone(),std::make_shared<Entry>(asString(heatingPayment)), heatingPayment().Clone()},
			{stageQuantities[0][10]()->Clone(),std::make_shared<Entry>(asString(advancedPayment)), advancedPayment().Clone()},
			{std::make_shared<Header>("ABC"),std::make_shared<Entry>(asString(payment)), std::make_shared<Quantity<Sum>>(payment())},
			{std::make_shared<Header>("ABC"),std::make_shared<Entry>(asString(resultSum)), std::make_shared<Quantity<Sum>>(resultSum)}
		};

		auto resultM = Init(costs)();
		auto div3 = std::make_unique<HtmlElements<DivTag>>("Div3",std::make_unique<Css<Style<GridArea,AreaNum<4>>>>());
		div3->Add(MatrixFormatter(resultM).Html());
		outs->push_back(std::move(div3));
		outs->push_back(std::move(div2));
		auto grid = HtmlElements<DivTag>{std::move(outs),std::make_unique<Css<Style<Display,Grid>, Style<Padding,Px<50>>, Style<GridTemplateAreas,DinA4>>>(), "grid-container"};
		html(grid);

        return resultElements.Col(6);
    }
	
	auto asString(const auto& val) const
	{
		std::stringstream ss;
		ss<<val;
		return ss.str();
	}
};
