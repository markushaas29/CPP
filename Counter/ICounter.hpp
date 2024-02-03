#include <memory>
#include <iostream>
#include "CounterDescription.hpp"
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

template<typename Desc, typename DM = Matrix<2, MatrixDescriptor<2, std::shared_ptr<IElement>>>>
class Counter: public ICounter
{
	using DataModel = DM;
	using DescriptorType = Desc;
	using Unit = typename Desc::Unit;
	using Type = Counter<Unit, DataModel>;
public:
	Counter(std::shared_ptr<Factory<IToken>> tf): tokenFactory{tf}  
	{
		auto elementTokens = (*tokenFactory)({{"SumToken"},{"EntryToken"},{"DateToken"},{"WorkToken"},{"VolumeToken"},{"ValueToken"}, {"EmptyToken"}});
        Matcher matcher(std::move(elementTokens));
		std::string f = "/home/markus/Downloads/CSV_TestFiles_2/BHot.csv";
		auto mvr = MatrixReader(f);
        auto mv = mvr.template M<2>();
		dataModel = std::make_unique<DataModel>(mv.Parse(matcher));
	}
	template<typename T>
	decltype(auto) To() const { return dataModel->template To<T>(); }
	decltype(auto) Accept(std::unique_ptr<BaseVisitor> bp) {  return dataModel->Accept(std::move(bp));   }
private:
	std::unique_ptr<DataModel> dataModel;
	std::shared_ptr<Factory<IToken>> tokenFactory;
	friend std::ostream& operator<<(std::ostream& s, const Counter& c) { return c.display(s);	}
	std::ostream& display(std::ostream& s) const { return s<<"\n"<<Unit::Name<<*dataModel; };
//	Derived cast() const 
//	{ 
//		 auto cderived = const_cast<Type&>(*this);
//         return  static_cast<Derived&>(cderived);
//	}
};


