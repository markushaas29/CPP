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
