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
class AdditionalCosts: public CalculatorBase<Quantity<Sum>, AdditionalCosts<S>>
{
	using Base = CalculatorBase<Quantity<Sum>, AdditionalCosts<S>>;
public:
	auto M() const { return parser->M().Rows(0, 1); }
	AdditionalCosts(std::shared_ptr<Factory<IToken>> fT,std::shared_ptr<Factory<IElement>> fE,std::shared_ptr<Factory<BaseVisitor>> fB, const std::string& p): 
		Base{fE,fB}, path{p}, tokenFactory{fT},parser{std::make_unique<AdditionalCostsParser>(tokenFactory,path)} {};
	const std::string path;
	std::shared_ptr<Factory<IToken>> tokenFactory;
	std::unique_ptr<IMatrixParser<2>> parser;
private:
	const std::string fileName = "AdditionalCosts.csv";
	typename Base::MatrixType exec(const Year& y, const HtmlBuilder<German>& f) 
	{
		auto m = (*parser)(true);
		std::vector<std::vector<std::shared_ptr<IElement>>> elements;
		Quantity<Sum> all;
		for(int i = 1; i < m.Rows(); ++i)
		{
			Quantity<Scalar> value{0};
			auto scalars = m.Cols(0,1,2,3,4,5)[1].To<Quantity<Scalar>>().Elements();
			std::for_each(scalars.cbegin(),scalars.cend(), [&value](const auto& i) { value = value + i; });
			
			auto proportion = std::make_shared<Quantity<Scalar>>(scalars[S::Index] / value);
			auto sum = std::make_shared<Quantity<Sum>>(*proportion * (*m[i][9]()).To<Quantity<Sum>>());
			all = all + *sum;
			elements.push_back({ m[i][9], proportion, sum}) ;
		}
	
		assert(all==Quantity<Sum>{69.50});
		elements.push_back({ std::make_shared<Quantity<Sum>>(all), std::make_shared<Empty>(), std::make_shared<Empty>()}) ;
		auto result = Init(elements)();

		return result;	}
};
