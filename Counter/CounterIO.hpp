#include "../Unit/Unit.hpp"
#include "../Descriptor/IDescriptor.hpp"
#include "../Matrix/Matrix.hpp"
#include "../Matrix/MatrixReader.hpp"

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
	friend T;
public:
	using Type = T;
	using DataModel = typename T::DataModel;
//	using Unit = E::Unit;
	CounterIO(std::shared_ptr<Factory<IToken>> f, const std::string& p): tokenFactory{f}, path{p} {}
//	inline static constexpr uint Number = No;
//	inline static constexpr const char* Entity = E::Name;
//	inline static constexpr const char* Stage = S::Value;
//	inline static std::string Identifier = std::string(Entity) + std::string(D::Value) + "_" + std::string(Stage) + "_" + std::to_string(No);
//	static std::unique_ptr<CounterIO> Make(std::shared_ptr<Factory<IToken>> f,const std::string& s) { return std::make_unique<CounterIO>(f,s);	}
//	virtual const std::string& Path() const { return path; };
private:	
	std::string path;
	std::shared_ptr<Factory<IToken>> tokenFactory;
	virtual std::unique_ptr<DataModel> read() const 
	{
        auto mvr = MatrixReader(path);   
        auto mv = mvr.template M<2>();
        auto elementTokens = (*tokenFactory)({{"SumToken"},{"EntryToken"},{"DateToken"},{"WorkToken"},{"VolumeToken"},{"ValueToken"}, {"EmptyToken"}});
        Matcher matcher(std::move(elementTokens));
        return std::make_unique<DataModel>(mv.Parse(matcher));
    }
	virtual std::ostream& display(std::ostream& out) const { 	return out;	}
};
