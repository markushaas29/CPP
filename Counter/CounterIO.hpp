#include "../Unit/Unit.hpp"
#include "../Descriptor/IDescriptor.hpp"
#include "../Matrix/Matrix.hpp"
#include "../Matrix/MatrixReader.hpp"
#include "../Matrix/MatrixParsers.hpp"

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
		auto mf1 = MatrixFormatter(*dm);
        auto out = mf1(DescriptorType::Identifier +".html",descriptor.Path());
		return dm;
	}
	virtual std::ostream& display(std::ostream& out) const { 	return out;	}
};
