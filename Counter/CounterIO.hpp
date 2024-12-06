#include "../Unit/Unit.hpp"
#include "../Descriptor/IDescriptor.hpp"
#include "../Matrix/Matrix.hpp"
#include "../Matrix/MatrixReader.hpp"
#include "../Matrix/MatrixParsers.hpp"
#include "../HTMLBuilder/HTMLBuilder.hpp"

#pragma once

template<typename T>
class ICounterIO
{
	friend T;
	using Type = T;
	using DataModel = typename T::DataModel;
	virtual std::unique_ptr<DataModel> read() const = 0;
	virtual std::ostream& display(std::ostream& out) const = 0;
};

template<typename T>
class CounterIO: public ICounterIO<T>
{
public:
	using Type = T;
	using DescriptorType = typename T::DescriptionType;
	using DataModel = typename T::DataModel;
	using Unit = typename T::Unit;
	CounterIO(std::shared_ptr<Factory<IToken>> f, const DescriptorType& d): descriptor{d}, parser{std::make_unique<CounterParser<Type>>(f,descriptor.Filename())} {}
private:	
	DescriptorType descriptor;
    std::unique_ptr<IMatrixParser<2>> parser;   
	virtual std::unique_ptr<DataModel> read() const 
	{ 
		auto dm = std::make_unique<DataModel>((*parser)()); 
		auto elements = dm->Elements();
		std::vector<std::vector<std::shared_ptr<IElement>>> newElements;
		
		std::unique_ptr<BaseVisitor> cvv = std::make_unique<ConsumptionVisitor<Quantity<Unit>>>();
		cvv = dm->Accept(std::move(cvv));
		auto cv = cvv->template As<ConsumptionVisitor<Quantity<Unit>>>();
		std::cout<<cv<<std::endl;

		size_t rows = elements.size() / 2;
		for(int i = 0; i <= rows; i = i + 2)
		{
			std::vector<std::shared_ptr<IElement>> nElements;
			auto nR = elements[i+1];
			nElements.push_back(elements[i]);
			nElements.push_back(elements[i+1]);
			auto oR = elements[i+3];
			nElements.push_back(std::make_shared<Quantity<Unit>>((nR->template To<Quantity<Unit>>() - oR->template To<Quantity<Unit>>())));

			newElements.push_back(nElements);
		}
		std::vector<std::shared_ptr<IElement>> nElements2;
		nElements2.insert(nElements2.end(), elements.end()-4, elements.end()-2);
		nElements2.insert(nElements2.end(), elements.end()-3, elements.end()-2);
		newElements.push_back(nElements2);
		std::vector<std::shared_ptr<IElement>> nElements;
		nElements.insert(nElements.end(), elements.end()-2, elements.end());
		nElements.insert(nElements.end(), elements.end()-1, elements.end());
		newElements.push_back(nElements);
		auto modell = Init(newElements)();
		//std::cout<<"DataModel "<<modell<<std::endl;
		auto mf1 = MatrixFormatter(*dm);
        HtmlBuilder(DescriptorType::Identifier +".html",descriptor.Path())(mf1());
		return dm;
	}
	virtual std::ostream& display(std::ostream& out) const { 	return out;	}
};
