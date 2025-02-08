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
#include "../CSV/Element.hpp"
#include "../CSV/NumericStyle.hpp"
#include "../CSV/Matcher.hpp"
#include "../Quantity/Quantity.hpp"
#include "../Functional/Functional.hpp"
#include "../Common/DateTimes.hpp"
#include "../Visitor/ElementVisitor.hpp"
#include "../Visitor/FuncVisitor.hpp"
#include "../Invoice/InvoiceCalculatorBase.hpp"
#include "../Invoice/AccountCalculator.hpp"
#include "../Invoice/Form.hpp"

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
	typename Base::MatrixType exec(const Year& y, const HtmlBuilder<German>& f)
	{
		Builder<ICounter,Counter, BottomHotDesc, BottomColdDesc, MiddleHotDesc, MiddleColdDesc,TopHotDesc, TopColdDesc> b;
		auto cV = b("/home/markus/Dokumente/cpp/CSV_Files", tokenFactory);

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

        return Matrix<Base::Order,typename Base::DescriptorType>(typename Base::DescriptorType({1,resQ.size()}),ToDataType(resQ));
	}
};

template<typename S>
class Hall: public CalculatorBase<Quantity<Sum>, Hall<S>>
{
	using Base = CalculatorBase<Quantity<Sum>, Hall<S>>;
public:
	auto M() const { return parser->M().Rows(0, 1); }
	Hall(std::shared_ptr<Factory<IToken>> fT,std::shared_ptr<Factory<IElement>> fE,std::shared_ptr<Factory<BaseVisitor>> fB, const std::string& p): 
		Base{fE,fB}, path{p}, tokenFactory{fT},parser{std::make_unique<StageParser>(tokenFactory,path)} {};
	const std::string path;
	std::shared_ptr<Factory<IToken>> tokenFactory;
	std::unique_ptr<IMatrixParser<2>> parser;
private:
	const std::string fileName = "Hall.csv";
	typename Base::MatrixType exec(const Year& y, const HtmlBuilder<German>& f) 
	{
		auto m = (*parser)(true);
		auto names = m.Col(2).Rows({4,8}).Elements();
		std::vector<std::string> name;
		std::vector<std::shared_ptr<IElement>> sums;
		typename Base::MatrixType address;

		for(auto n : names)
			if(std::find_if(name.begin(), name.end(), [&n](const auto& i) { return n->Data() == i; }) == name.end())
				name.push_back(n->Data());
		
		auto q = Quantity<Sum>{0};
		std::vector<std::vector<std::shared_ptr<IElement>>> elements = {{std::make_shared<Header>("sumPerArea"),std::make_shared<Header>("Width"),std::make_shared<Header>("Length"),std::make_shared<Header>("area"),std::make_shared<Header>("monthly rent"),std::make_shared<Header>("annual rent")}};
		auto divs = std::make_unique<HtmlElements<DivTag>>("Div1","",std::make_unique<Css<Style<BackgroundColor,Hex<"ffffff">>>>());
		divs->Add(Html<DivTag>(Header{"Mietvetrag"},std::make_unique<Css<Style<FontWeight,Bold>, Style<MarginTop,Px<200>>, Style<FontSize,Px<25>>>>(),"Header", "Header"));

		std::vector<std::vector<std::shared_ptr<IElement>>> dates ={
			{std::make_shared<Entry>("Mietstart"),Date{1,1,2025}.Clone()},
		};
		divs->Add(MatrixFormatter(Init(dates)()).Lines(std::make_unique<Css<Style<PaddingTop,Px<180>>,Style<PaddingBottom,Px<180>>,Style<FontSize,Px<25>>>>()));

		for(auto i = 4; i < m.Rows();++i)
		{
			std::unique_ptr<BaseVisitor> fc = std::make_unique<ComposedFuncVisitor<Quantity<SumPerArea>, FuncVisitor<QL,QL,Mul>,Mul>>();
			std::unique_ptr<BaseVisitor> baseVisitor = std::make_unique<ElementCollector<Prename, Name, Street, StreetNumber, Postcode, Town>>();
			std::unique_ptr<BaseVisitor> slwVis = std::make_unique<ElementCollector<Quantity<SumPerArea>, Quantity<Length>>>();
			if(m[i][2]()->Data()==S::Name)
			{
				auto mfPre = MatrixFormatter(m.Cols(std::string("Pre"),std::string("Name"),std::string("Street"),std::string("Streetnumber"),std::string("Town"),std::string("Postcode"))[i].Transform<2>(3,2));
				fc = m[i].Accept(std::move(fc));

				auto fC = fc->template As<ComposedFuncVisitor<Quantity<SumPerArea>, FuncVisitor<QL,QL,Mul>,Mul>>();
				std::unique_ptr<BaseVisitor> fa = std::make_unique<FuncVisitor<QL,QL,Mul>>();
				auto sum = Mul{Constant{QSC{12}},fC.F()}();
				q = q + sum;
				sums.push_back(sum.Clone());
			    baseVisitor = m[i].Accept(std::move(baseVisitor));
				fa = m[i].Accept(std::move(fa));
				auto addressElements = baseVisitor->template Cast<ElementCollector<Prename, Name, Street, StreetNumber, Postcode, Town>>();
				address = Init(addressElements->Elements())().template Transform<2>(3,2);
				
			    slwVis = m[i].Accept(std::move(slwVis));
				auto properties = (slwVis->template Cast<ElementCollector<Quantity<SumPerArea>, Quantity<Length>>>())->Elements();
				auto fA = fa->template Cast<FuncVisitor<QL,QL,Mul>>();
				properties.push_back((*fA)().Clone());
				properties.push_back(std::make_shared<Entry>(this->asString(fC())));
				properties.push_back(q.Clone());

				elements.push_back(properties);
				
				auto outs = std::make_unique<std::vector<std::unique_ptr<IHtmlElement>>>();
				auto classCss = std::make_unique<StyleElement>();
				classCss->Add(std::make_unique<ClassCss<Style<Padding,Px<14>>>>("border"));
				outs->push_back(std::move(classCss));

				auto div0 = std::make_unique<HtmlElements<DivTag>>("Div0","",std::make_unique<Css<Style<GridArea,AreaNum<1>>,Style<TextAlign, Right>>>());
				div0->Add(Mul{Constant{QSC{12}},fC.F()}().Html());
				outs->push_back(std::move(div0));
				auto grid = HtmlElements<DivTag>{std::move(outs),std::make_unique<Css<Style<Display,Grid>, Style<Padding,Px<50>>>>(), "grid-container"};
			}
		}
		divs->Add(MatrixFormatter(Init(elements)()).Html(std::make_unique<Css<Style<PaddingTop,Px<180>>,Style<Width,Px<1100>>,Style<FontSize,Px<25>>>>()));

		auto inv = Contract(MatrixFormatter(address).Lines(),std::move(divs),path, std::string(S::Name));
		inv.exec();

		std::vector<std::vector<std::shared_ptr<IElement>>> s = {sums, sums};
		return Init(s)();	}
};

template<typename S>
class StageBase: public CalculatorBase<Quantity<Sum>, StageBase<S>>
{
	using Base = CalculatorBase<Quantity<Sum>, StageBase<S>>;
public:
	auto M() const { return parser->M().Rows(0, S::Index); }
protected:
	StageBase(std::shared_ptr<ICalculator<Quantity<Sum>>> acc, std::shared_ptr<Factory<IToken>> fT,std::shared_ptr<Factory<IElement>> fE,std::shared_ptr<Factory<BaseVisitor>> fB, const std::string& p): 
		Base{fE,fB}, account{acc}, path{p}, tokenFactory{fT},parser{std::make_unique<StageParser>(tokenFactory,path)} { };
	const std::string path;
	std::shared_ptr<Factory<IToken>> tokenFactory;
	std::unique_ptr<IMatrixParser<2>> parser;
	std::shared_ptr<ICalculator<Quantity<Sum>>> account;
private:
	const std::string fileName = "SN_Name.csv";
	typename Base::MatrixType exec(const Year& y, const HtmlBuilder<German>& f) {	return matrix(f,y);	}
	virtual typename Base::MatrixType matrix(const HtmlBuilder<German>& f, const Year& y) const = 0;
};

template<typename S>
class ExtraCostsCalculator: public StageBase<S>
{
	using Base = StageBase<S>;
public:
	ExtraCostsCalculator(std::shared_ptr<ICalculator<Quantity<Sum>>> acc, std::shared_ptr<Factory<IToken>> fT,std::shared_ptr<Factory<IElement>> fE,std::shared_ptr<Factory<BaseVisitor>> fB, const std::string& p): 
		Base{acc, fT,fE,fB, p}, properties((*Base::parser)(true).Rows(0,S::Index)), advancePayment{(properties[1][14].template As<Quantity<Sum>>()+properties[1][15].template As<Quantity<Sum>>()) * Quantity<Scalar>{12}} {};
	auto AdvancePayment() { return advancePayment; }
	auto Properties() { return properties; }
	auto AdvanceItems() { return properties.Cols(13,14,15); }
	auto Result(const Year& y) { return this->Value(y) + advancePayment; }
	auto Costs(const Year& y, const HtmlBuilder<German>& html = HtmlBuilder<German>("") ) const { return (*Base::account)(y, html); }
private:
	Matrix<2, MatrixDescriptor<2,typename Base::ElementType>> properties;
	Quantity<Sum> advancePayment;
	virtual typename Base::MatrixType matrix(const HtmlBuilder<German>& f, const Year& y) const
	{
		auto stageMatrix = (*Base::parser)().Cols(7,8,9,10,11,12).template To<Quantity<Scalar>>();
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
            stageMatrix = stageMatrix.Set(readings(y,f)[0][0].template As<Quantity<Scalar>>(),Type::Index-1,((int)stageMatrix.Cols()-1));
            return process<N+1,Tup>(stageMatrix,fT,fE,fB,p,f,y);
        }
    }
    
    template<size_t N, typename Tup>
    auto calcCosts(auto stageMatrix, std::shared_ptr<Factory<IToken>> tokenFactory,std::shared_ptr<Factory<IElement>> elementFactory,std::shared_ptr<Factory<BaseVisitor>> visitorFactory, const std::string& path, const HtmlBuilder<German>& f, const Year& y) const
    {
        stageMatrix = process<0,Tup>(stageMatrix,tokenFactory,elementFactory,visitorFactory, path, f, y);
		auto divs = std::make_unique<HtmlElements<DivTag>>("Div1","",std::make_unique<Css<Style<BackgroundColor,Hex<"ffffff">>>>());
		divs->Add(Html<DivTag>(Header{"Nebenkosten"},std::make_unique<Css<Style<FontWeight,Bold>, Style<MarginTop,Px<200>>, Style<FontSize,Px<25>>>>(),"Header", "Header"));
        
		auto html = HtmlBuilder(std::to_string(S::Index)+"_"+y.ToString()+".html","/home/markus/Dokumente/cpp/CSV_Files");

		auto stageproperties = (*Base::parser)(true).Rows(0,S::Index);
	
		auto heads = std::make_unique<std::vector<std::unique_ptr<IHtmlElement>>>(); 
        heads->push_back(std::make_unique<LinkElement>());
        html.Add(std::make_unique<HtmlElements<Head>>(std::move(heads)));
		
		std::unique_ptr<BaseVisitor> baseVisitor = std::make_unique<ElementCollector<Prename, Name, Street, StreetNumber, Postcode, Town>>();
	    baseVisitor = stageproperties[1].Accept(std::move(baseVisitor));
		auto addressElements = baseVisitor->template Cast<ElementCollector<Prename, Name, Street, StreetNumber, Postcode, Town>>();
		auto address = Init(addressElements->Elements())().template Transform<2>(3,2);
	
		std::unique_ptr<BaseVisitor> baseVisitor2 = std::make_unique<ElementCollector<Quantity<Sum>>>();
	    baseVisitor2 = stageproperties[1].Accept(std::move(baseVisitor2));
		auto sumElements = baseVisitor2->template Cast<ElementCollector<Quantity<Sum>>>();
		auto rents = Init(sumElements->Elements())();
		
		auto stageQT = stageproperties^-1;
		auto mf = MatrixFormatter(stageQT.Rows({13,16}));

		auto outs = std::make_unique<std::vector<std::unique_ptr<IHtmlElement>>>();
		auto divA = std::make_unique<HtmlElements<DivTag>>("Div0","",std::make_unique<Css<Style<Margin,Px<10>>,Style<BackgroundColor,Hex<"ffffff">>,Style<TextAlign, Left>>>());
		divA->Add(MatrixFormatter(address).Lines(std::make_unique<Css<Style<FontWeight,Bold>>>(),"Address", "AddressId"));
		outs->push_back(std::move(divA));

		auto classCss = std::make_unique<StyleElement>();
		classCss->Add(std::make_unique<ClassCss<Style<Padding,Px<14>>>>("border"));
		outs->push_back(std::move(classCss));
		auto div0 = std::make_unique<HtmlElements<DivTag>>("Div0","",std::make_unique<Css<Style<TextAlign, Right>>>());
		div0->Add(Html<P>(Date::Today(),std::make_unique<Css<Style<FontWeight,Bold>,Style<FontSize,Px<25>>>>(),"Date", "DateId"));
		outs->push_back(std::move(div0));
		
		auto div1 = std::make_unique<HtmlElements<DivTag>>("Div1","",nullptr);
		auto div11 = std::make_unique<HtmlElements<DivTag>>("Div11","",std::make_unique<Css<Style<Margin,Px<10>>,Style<BackgroundColor,Hex<"f9f9f9">>, Style<FloatTag,Left>>>());
		div11->Add(mf.Html(std::make_unique<HtmlElement<Caption, Header>>(Header("Payments")),nullptr,"Sums", "ExtraCosts"));
		div1->Add(std::move(div11));

		auto extraCosts = stageproperties[1].Rows(14,15).template To<Quantity<Sum>>();
  		auto yearCosts = (extraCosts) * Quantity<Scalar>{12};
		auto heatingPayment = yearCosts[0];
		auto advancedPayment = yearCosts[1];

		auto payment = yearCosts.ColSum();
		std::vector<std::vector<std::shared_ptr<IElement>>> annualAdvancePayment = 
		{
			{stageproperties[0][14]()->Clone(),std::make_shared<Entry>(this->asString(heatingPayment)), heatingPayment().Clone()},
			{stageproperties[0][15]()->Clone(),std::make_shared<Entry>(this->asString(advancedPayment)), advancedPayment().Clone()},
			{std::make_shared<Header>("Advance"),std::make_shared<Entry>(this->asString(payment)), std::make_shared<Quantity<Sum>>(payment())},
		};

		auto annualAdvancePaymentForm = MatrixFormatter(Init(annualAdvancePayment)());
		auto div5 = std::make_unique<HtmlElements<DivTag>>("Div5","",std::make_unique<Css<Style<Margin,Px<10>>, Style<FloatTag,Right>>>());
		div5->Add(annualAdvancePaymentForm.Html(std::make_unique<HtmlElement<Caption, Header>>(Header("annualStatement")),nullptr,"Sums", "Payments"));
		div1->Add(std::move(div5));
		outs->push_back(std::move(div1));

        auto accountM = Costs(y, html);  
        auto sumMatrix = accountM[0].template To<Quantity<Sum>>();  
        auto names = accountM[1];  
        auto csum = stageMatrix.ColSum()();
        auto stagesDiv = (stageMatrix / csum());

		auto result = stagesDiv[S::Index-1] * sumMatrix;
		auto res = result().template To<Quantity<Sum>>();

		std::vector<std::vector<std::shared_ptr<IElement>>> vp;
		std::vector<std::string> dividers = {"Persons","Area","Proportion","Proportion","Area","Counter"};
		for(size_t i = 0; i < 6; ++i)
		{
			std::vector<std::shared_ptr<IElement>> vpr;
			vpr.push_back(std::make_shared<Header>(names[i]()->Data()));
			vpr.push_back(sumMatrix[i].Get().template To<Quantity<Sum>>().Clone());
			vpr.push_back(std::make_shared<Entry>(dividers[i]));
			vpr.push_back(std::make_shared<Quantity<Scalar,Pure,double>>(this->asString(stageMatrix[S::Index-1][i])));
			vpr.push_back(csum[i].Get().Clone());
			vpr.push_back(std::make_shared<Entry>(this->asString(stagesDiv[S::Index-1][i])));
			vpr.push_back(stagesDiv[S::Index-1][i].Get().Clone());
			vpr.push_back(std::make_shared<Entry>(this->asString(result[i][i])));
			vpr.push_back(res[i][i].Get().template To<Quantity<Sum>>().Clone());
			vp.push_back(vpr);
		}

		auto resultMatrix = Init(vp)();
		auto div2 = std::make_unique<HtmlElements<DivTag>>("Div1","",std::make_unique<Css<Style<Margin,Px<10>>,Style<BackgroundColor,Hex<"f9f9f9">>>>());
		div2->Add(appendHeaders({"Name","Costs","Divider","Proportion","Whole","Calculation","Result","Calculation","Result"}, vp).Html(std::make_unique<HtmlElement<Caption, Header>>(Header("Payments")),nullptr,"Sums","Costs"));
		
		auto sumX = resultMatrix.Col(8).template To<Quantity<Sum>>().ColSum();
		std::vector<std::vector<std::shared_ptr<IElement>>> addedCosts = {{std::make_shared<Header>("proportional costs"),std::make_shared<Entry>(this->asString(sumX)), std::make_shared<Quantity<Sum>>(sumX())}	};
		auto mAddedCosts = MatrixFormatter(Init(addedCosts)());
		auto div4 = std::make_unique<HtmlElements<DivTag>>("Div4","",std::make_unique<Css<Style<Margin,Px<10>>>>());
		div4->Add(mAddedCosts.Html(nullptr,"Sums","Costs2"));

		auto sumCol = resultMatrix.Col(8);
		auto sum = sumCol.template To<Quantity<Sum>>().ColSum();
		auto resultSum = Constant(Quantity<Sum>{sum()}) + Constant(Quantity<Sum>{payment()});

		std::vector<std::vector<std::shared_ptr<IHtmlElement>>> costs = 
		{
			{Html<Td>(Header{"total"} ,std::make_unique<Css<Style<FontSize,Px<40>>, Style<FontWeight,Bold>>>()),
			Html<Td>(Entry{this->asString(resultSum)} ,std::make_unique<Css<Style<FontSize,Px<40>>, Style<FontWeight,Bold>>>()),  
			Html<Td>(QS{resultSum} ,std::make_unique<Css<Style<FontSize,Px<40>>, Style<FontWeight,Bold>>>())}
		};

		auto mCostsForm = MatrixFormatter(Init(costs)());

		auto div3 = std::make_unique<HtmlElements<DivTag>>("Div3","",std::make_unique<Css<Style<Margin,Px<10>>>>());
		div3->Add(mCostsForm.Html(nullptr,"Sums","Total"));
		outs->push_back(std::move(div2));
		outs->push_back(std::move(div4));
		outs->push_back(std::move(div3));
		auto grid = std::make_unique<HtmlElements<DivTag>>(std::move(outs),std::make_unique<Css<Style<Display,Grid>, Style<Padding,Px<10>>>>(), "grid-container");
		auto body = std::make_unique<HtmlElement<Body,IHtmlElement>>(std::move(grid),std::make_unique<Css<Style<Margin,Px<10>>, Style<FloatTag,Left>>>(),"mainBody");
		html.Add(std::move(body));

        auto v = sumCol.Elements();
		
		divs->Add(Html<DivTag>(Text{"Die Nebenkostenabrechnung Zeitraum: 01.01.2024 bis 31.12.2024 hat abzüglich Ihrer Vorauszahlungen ein Minus von ergeben."},std::make_unique<Css<Style<MarginTop,Px<100>>, Style<FontSize,Px<25>>>>()));
		divs->Add(Html<DivTag>(resultSum(),std::make_unique<Css<Style<FontWeight,Bold>, Style<Margin,Px<150>>, Style<FontSize,Px<40>>>>()));
		divs->Add(Html<DivTag>(Text{"Bitte überweisen Sie den Betrag auf das unten genannte Konto."},std::make_unique<Css<Style<MarginTop,Px<200>>, Style<FontSize,Px<25>>>>()));
		auto inv = ExtraCostInvoice(MatrixFormatter(address).Lines(),std::move(divs),path, std::to_string(S::Index)+"_"+y.ToString()+"_Nebenkosten");
		inv.exec();
		
        return Matrix<Base::Order,typename Base::DescriptorType>(typename Base::DescriptorType({1,v.size()}),v);
    }
	
	auto appendHeaders(const std::vector<std::string>& headers, const auto& vp) const
	{
		std::vector<std::shared_ptr<IElement>> first;
		for(size_t i = 0; i < headers.size(); ++i)
			first.push_back(std::make_shared<Header>(headers[i]));

		std::vector<std::vector<std::shared_ptr<IElement>>> vph = { first };

		vph.insert(vph.end(), vp.begin(), vp.end());
		return MatrixFormatter(Init(vph)());
	}
};
