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
		std::vector<std::shared_ptr<IElement>> newElements;
		
		size_t rows = elements.size() / 2;
		for(int i = 0; i <= rows; i = i + 2)
		{
			auto nR = elements[i+1];
			newElements.push_back(elements[i]);
			newElements.push_back(elements[i+1]);
			auto oR = elements[i+3];
			auto diff = std::make_shared<Quantity<Unit>>((nR->template To<Quantity<Unit>>() - oR->template To<Quantity<Unit>>()));
			newElements.push_back(std::make_shared<Quantity<Unit>>((nR->template To<Quantity<Unit>>() - oR->template To<Quantity<Unit>>())));
			std::cout<<"DataModel "<<*oR<<"\t"<<*nR<<"\t"<<*diff<<std::endl;
		}
		newElements.insert(newElements.end(), elements.end()-2, elements.end());
		newElements.insert(newElements.end(), elements.end()-1, elements.end());
		for(int i = 0; i < newElements.size(); ++i)
		{
			std::cout<<"D "<<*newElements[i]<<std::endl;
		}
		auto mf1 = MatrixFormatter(*dm);
        HtmlBuilder(DescriptorType::Identifier +".html",descriptor.Path())(mf1());
		return dm;
	}
	virtual std::ostream& display(std::ostream& out) const { 	return out;	}
};
