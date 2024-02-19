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
	CounterIO(std::shared_ptr<Factory<IToken>> f, const std::string& p): tokenFactory{f}, path{p}, parser{std::make_unique<CounterParser<Type>>(f,path)}, reader{std::make_unique<MatrixReader<std::string>>(path)} {}
private:	
	std::shared_ptr<Factory<IToken>> tokenFactory;
	std::string path;
    std::unique_ptr<MatrixReader<std::string>> reader;   
    std::unique_ptr<IMatrixParser<2>> parser;   
	virtual std::unique_ptr<DataModel> read() const 
	{
        auto mv = (*reader).template M<2>();
        auto elementTokens = (*tokenFactory)({{"DateToken"},{ Unit::TokenName }});
        Matcher matcher(std::move(elementTokens));
        return std::make_unique<DataModel>(mv.Parse(matcher));
    }
	virtual std::ostream& display(std::ostream& out) const { 	return out;	}
};
