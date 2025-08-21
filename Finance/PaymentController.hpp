#include <fstream>
#include <sstream>
#include <vector> 
#include <memory> 
#include "../Matrix/Matrix.hpp"
#include "../Matrix/MatrixReader.hpp"
#include "../Matrix/MatrixDescriptor.hpp"
#include "../Matrix/MatrixComposition.hpp"
#include "../Matrix/M3.hpp"
#include "../Matrix/MatrixParsers.hpp"
#include "../Matrix/MatrixFormatter.hpp"
#include "../HTMLBuilder/HTMLBuilder.hpp"
#include "../Common/DateTimes.hpp"
#include "../CSV/Elements.hpp"
#include "../CSV/Element.hpp"
#include "../CSV/NumericStyle.hpp"
#include "../CSV/Matcher.hpp"
#include "../Quantity/Quantity.hpp"
#include "../Functional/Functional.hpp"
#include "../Common/DateTimes.hpp"

#pragma once

class PaymentController
{
	static auto createCategories()
    {
		std::unique_ptr<std::vector<std::unique_ptr<PaymentCategory>>> res = std::make_unique<std::vector<std::unique_ptr<PaymentCategory>>>();
		res->push_back(std::make_unique<PaymentCategory>(Quantity<Scalar>{0.1}, "Retirement Provision"));
		res->push_back(std::make_unique<PaymentCategory>(Quantity<Scalar>{0.1}, "Long term"));
		res->push_back(std::make_unique<PaymentCategory>(Quantity<Scalar>{0.125}, "Donation"));
		res->push_back(std::make_unique<PaymentCategory>(Quantity<Scalar>{0.55}, "Livelihood"));
		res->push_back(std::make_unique<PaymentCategory>(Quantity<Scalar>{0.125}, "Miscellaneous"));
		return res;
	}
public:
	PaymentController(): minus{createCategories()}{} 
	auto operator()() { execute(); }
private:
	Quantity<Sum> advancePayment;
	std::unique_ptr<PaymentCategory> plus = std::make_unique<PaymentCategory>(Quantity<Scalar>{1}, "Plus");
	std::unique_ptr<std::vector<std::unique_ptr<PaymentCategory>>> minus = std::make_unique<std::vector<std::unique_ptr<PaymentCategory>>>();
    
	void execute()
    {
		auto divs = std::make_unique<HtmlElements<DivTag>>("Div1","",std::make_unique<Css<Style<BackgroundColor,Hex<"ffffff">>>>());
		divs->Add(Html<DivTag>(Header{"Nebenkosten"},std::make_unique<Css<Style<FontWeight,Bold>, Style<MarginTop,Px<200>>, Style<FontSize,Px<25>>>>(),"Header", "Header"));
        
		auto html = HtmlBuilder("_"+Year{2025}.ToString()+".html","/home/markus/Downloads");

//		auto stageproperties = (*Base::parser)(true).Rows(0,S::Index);
//	
		auto heads = std::make_unique<std::vector<std::unique_ptr<IHtmlElement>>>(); 
        heads->push_back(std::make_unique<LinkElement>());
        html.Add(std::make_unique<HtmlElements<Head>>(std::move(heads)));
//		
//		std::unique_ptr<BaseVisitor> baseVisitor = std::make_unique<ElementCollector<Prename, Name, Street, StreetNumber, Postcode, Town>>();
//	    baseVisitor = stageproperties[1].Accept(std::move(baseVisitor));
//		auto addressElements = baseVisitor->template Cast<ElementCollector<Prename, Name, Street, StreetNumber, Postcode, Town>>();
//		auto address = Init(addressElements->Elements())().template Transform<2>(3,2);
//	
//		std::unique_ptr<BaseVisitor> baseVisitor2 = std::make_unique<ElementCollector<Quantity<Sum>>>();
//	    baseVisitor2 = stageproperties[1].Accept(std::move(baseVisitor2));
//		auto sumElements = baseVisitor2->template Cast<ElementCollector<Quantity<Sum>>>();
//		auto rents = Init(sumElements->Elements())();
//		
//		auto stageQT = stageproperties^-1;
//		auto mf = MatrixFormatter(stageQT.Rows({13,16}));
//
		auto outs = std::make_unique<std::vector<std::unique_ptr<IHtmlElement>>>();
		auto divA = std::make_unique<HtmlElements<DivTag>>("Div0","",std::make_unique<Css<Style<Margin,Px<10>>,Style<BackgroundColor,Hex<"ffffff">>,Style<TextAlign, Left>>>());
		divA->Add((*plus)().Html());
		outs->push_back(std::move(divA));
		auto grid = std::make_unique<HtmlElements<DivTag>>(std::move(outs),std::make_unique<Css<Style<Display,Grid>, Style<Padding,Px<10>>>>(), "grid-container");
		auto body = std::make_unique<HtmlElement<Body,IHtmlElement>>(std::move(grid),std::make_unique<Css<Style<Margin,Px<10>>, Style<FloatTag,Left>>>(),"mainBody");
		html.Add(std::move(body));
//
//		auto resultMatrix = Init(vp)();
//		auto div2 = std::make_unique<HtmlElements<DivTag>>("Div1","",std::make_unique<Css<Style<Margin,Px<10>>,Style<BackgroundColor,Hex<"f9f9f9">>>>());
//		div2->Add(appendHeaders({"Name","Costs","Divider","Proportion","Whole","Calculation","Result","Calculation","Result"}, vp).Html(std::make_unique<HtmlElement<Caption, Header>>(Header("Payments")),nullptr,"Sums","Costs"));
//		
//		auto sumX = resultMatrix.Col(8).template To<Quantity<Sum>>().ColSum();
//		std::vector<std::vector<std::shared_ptr<IElement>>> addedCosts = {{std::make_shared<Header>("proportional costs"),std::make_shared<Entry>(this->asString(sumX)), std::make_shared<Quantity<Sum>>(sumX())}	};
//		auto mAddedCosts = MatrixFormatter(Init(addedCosts)());
//		auto div4 = std::make_unique<HtmlElements<DivTag>>("Div4","",std::make_unique<Css<Style<Margin,Px<10>>>>());
//		div4->Add(mAddedCosts.Html(nullptr,"Sums","Costs2"));
//
//		auto sumCol = resultMatrix.Col(8);
//		auto sum = sumCol.template To<Quantity<Sum>>().ColSum();
//		auto resultSum = Constant(Quantity<Sum>{sum()}) + Constant(Quantity<Sum>{payment()});
//
//		std::vector<std::vector<std::shared_ptr<IHtmlElement>>> costs = 
//		{
//			{Html<Td>(Header{"total"} ,std::make_unique<Css<Style<FontSize,Px<40>>, Style<FontWeight,Bold>>>()),
//			Html<Td>(Entry{this->asString(resultSum)} ,std::make_unique<Css<Style<FontSize,Px<40>>, Style<FontWeight,Bold>>>()),  
//			Html<Td>(QS{resultSum} ,std::make_unique<Css<Style<FontSize,Px<40>>, Style<FontWeight,Bold>>>())}
//		};
//
//		auto mCostsForm = MatrixFormatter(Init(costs)());
//
//		auto div3 = std::make_unique<HtmlElements<DivTag>>("Div3","",std::make_unique<Css<Style<Margin,Px<10>>>>());
//		div3->Add(mCostsForm.Html(nullptr,"Sums","Total"));
//		outs->push_back(std::move(div2));
//		outs->push_back(std::move(div4));
//		outs->push_back(std::move(div3));
//		auto grid = std::make_unique<HtmlElements<DivTag>>(std::move(outs),std::make_unique<Css<Style<Display,Grid>, Style<Padding,Px<10>>>>(), "grid-container");
//		auto body = std::make_unique<HtmlElement<Body,IHtmlElement>>(std::move(grid),std::make_unique<Css<Style<Margin,Px<10>>, Style<FloatTag,Left>>>(),"mainBody");
//		html.Add(std::move(body));
//
//        auto v = sumCol.Elements();
//		
//		divs->Add(Html<DivTag>(Text{"Die Nebenkostenabrechnung Zeitraum: 01.01.2024 bis 31.12.2024 hat abzüglich Ihrer Vorauszahlungen ein Minus von ergeben."},std::make_unique<Css<Style<MarginTop,Px<100>>, Style<FontSize,Px<25>>>>()));
//		divs->Add(Html<DivTag>(resultSum(),std::make_unique<Css<Style<FontWeight,Bold>, Style<Margin,Px<100>>, Style<FontSize,Px<40>>>>()));
//		divs->Add(Html<DivTag>(Text{"Bitte überweisen Sie den Betrag auf das unten genannte Konto."},std::make_unique<Css<Style<MarginTop,Px<100>>, Style<FontSize,Px<25>>>>()));
//		auto inv = ExtraCostInvoice(MatrixFormatter(address).Lines(),std::move(divs),path, std::to_string(S::Index)+"_"+y.ToString()+"_Nebenkosten");
//		inv.exec();
//		
//        return Matrix<Base::Order,typename Base::DescriptorType>(typename Base::DescriptorType({1,v.size()}),v);
		html();
    }
	
//	auto appendHeaders(const std::vector<std::string>& headers, const auto& vp) const
//	{
//		std::vector<std::shared_ptr<IElement>> first;
//		for(size_t i = 0; i < headers.size(); ++i)
//			first.push_back(std::make_shared<Header>(headers[i]));
//
//		std::vector<std::vector<std::shared_ptr<IElement>>> vph = { first };
//
//		vph.insert(vph.end(), vp.begin(), vp.end());
//		return MatrixFormatter(Init(vph)());
//	}
};
