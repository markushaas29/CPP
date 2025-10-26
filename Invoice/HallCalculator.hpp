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
#include "../Invoice/ExtraCostsCalculator.hpp"
#include "../Invoice/Form.hpp"

#pragma once

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
				properties.push_back(sum.Clone());

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
		
		std::vector<std::shared_ptr<IElement>> els = {std::make_shared<Header>("annual rent"), q.Clone()};
		divs->Add(MatrixFormatter(Init(els)()).Html(std::make_unique<Css<Style<PaddingTop,Px<180>>,Style<Width,Px<200>>,Style<FontSize,Px<25>>>>()));

		auto hallCurrent = std::make_unique<HallCurrentCalculator>(tokenFactory,Base::elementFactory,Base::visitorFactory,path);

		std::vector<std::vector<std::shared_ptr<IElement>>> currents ={
			{ std::make_shared<Header>("Gesamt"), hallCurrent->Value(y).Clone()},
			{ std::make_shared<Header>("Gesamt"), std::make_shared<Quantity<Sum>>(Quantity<Scalar>{1. / 3} * hallCurrent->Value(y))}
		};
		auto hc = Init(currents)();
		(*hallCurrent)(y,f);

		auto inv = Contract(MatrixFormatter(address).Lines(),std::move(divs),path, std::string(S::Name));
		inv.exec();
	
		divs = std::make_unique<HtmlElements<DivTag>>("Div1","",std::make_unique<Css<Style<BackgroundColor,Hex<"ffffff">>>>());
		divs->Add(Html<DivTag>(Text{"Die Nebenkostenabrechnung Zeitraum: 01.01.2024 bis 31.12.2024 hat abzüglich Ihrer Vorauszahlungen ein Minus von ergeben."},std::make_unique<Css<Style<MarginTop,Px<250>>, Style<FontSize,Px<25>>>>()));
		divs->Add(MatrixFormatter(hc).Html(std::make_unique<Css<Style<MarginTop,Px<100>>, Style<FontSize,Px<25>>>>()));
		divs->Add(Html<DivTag>(Text{"Bitte überweisen Sie den Betrag auf das unten genannte Konto."},std::make_unique<Css<Style<MarginTop,Px<100>>, Style<FontSize,Px<25>>>>()));
		auto inv2 = Invoice(MatrixFormatter(address).Lines(),std::move(divs),path, std::string(S::Name)+"_"+std::string(Year{2025}));
		inv2.exec();

		std::vector<std::vector<std::shared_ptr<IElement>>> s = {sums, sums};
		return Init(s)();	}
};
