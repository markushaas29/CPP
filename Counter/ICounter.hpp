#include <memory>
#include <iostream>
#include "../Matrix/Matrix.hpp"
#include "../Matrix/MatrixReader.hpp"

#pragma once

class ICounter
{
public:
	virtual ~ICounter() {};
private:
	virtual std::ostream& display(std::ostream& s) const = 0;
};

template<typename L, typename R, typename DM = Matrix<2, MatrixDescriptor<2, std::shared_ptr<IElement>>>>
class CounterBase: public ICounter
{
	//using Derived = D<L,R>;
	using DataModel = DM;
	using Type = CounterBase<L,R, DataModel>;
public:
	CounterBase(std::shared_ptr<Factory<IToken>> tf): tokenFactory{tf}  
	{
		auto elementTokens = (*tokenFactory)({{"SumToken"},{"EntryToken"},{"DateToken"},{"WorkToken"},{"VolumeToken"},{"ValueToken"}, {"EmptyToken"}});
        Matcher matcher(std::move(elementTokens));
		std::string f = "/home/markus/Downloads/CSV_TestFiles_2/THot.csv";
		auto mvr = MatrixReader(f);
        auto mv = mvr.template M<2>();
		dataModel = std::make_unique<DataModel>(mv.Parse(matcher));
	}
private:
	std::unique_ptr<DataModel> dataModel;
	std::shared_ptr<Factory<IToken>> tokenFactory;
	friend std::ostream& operator<<(std::ostream& s, const CounterBase& c) { return c.display(s);	}
	std::ostream& display(std::ostream& s) const { return s<<*dataModel; };
//	Derived cast() const 
//	{ 
//		 auto cderived = const_cast<Type&>(*this);
//         return  static_cast<Derived&>(cderived);
//	}
};


