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
	using DataModel = typename T::DataModel;
	using Unit = typename T::Unit;
	CounterIO(std::shared_ptr<Factory<IToken>> f, const std::string& p): parser{std::make_unique<CounterParser<Type>>(f,p)} {}
private:	
    std::unique_ptr<IMatrixParser<2>> parser;   
	virtual std::unique_ptr<DataModel> read() const {  return std::make_unique<DataModel>((*parser)()); }
	virtual std::ostream& display(std::ostream& out) const { 	return out;	}
};
