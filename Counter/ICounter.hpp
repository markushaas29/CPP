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

template<typename Desc>
class Counter: public ICounter
{
	using DescriptionType = Desc;
	using DataModel = typename Desc::DataModel;
	using Unit = typename Desc::Unit;
	using Type = Counter<Desc>;
public:
	Counter(std::string s, std::shared_ptr<Factory<IToken>> tf): Counter{DescriptionType::Make(tf,s),tf} {}
	Counter(std::unique_ptr<DescriptionType> d, std::shared_ptr<Factory<IToken>> tf): descriptor{std::move(d)}, tokenFactory{tf}, dataModel{descriptor->Read()}
	{
		auto elementTokens = (*tokenFactory)({{"SumToken"},{"EntryToken"},{"DateToken"},{"WorkToken"},{"VolumeToken"},{"ValueToken"}, {"EmptyToken"}});
        Matcher matcher(std::move(elementTokens));
		auto mvr = MatrixReader(descriptor->Path());
        auto mv = mvr.template M<2>();
		dataModel = std::make_unique<DataModel>(mv.Parse(matcher));
	}
	template<typename T>
	decltype(auto) To() const { return dataModel->template To<T>(); }
	decltype(auto) Accept(std::unique_ptr<BaseVisitor> bp) {  return dataModel->Accept(std::move(bp));   }
private:
	std::unique_ptr<DescriptionType> descriptor;
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


