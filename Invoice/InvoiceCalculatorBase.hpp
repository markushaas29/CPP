#include <iostream>
#include <cassert> 
#include <vector> 
#include <memory> 
#include "../Matrix/Matrix.hpp"
#include "../Matrix/MatrixReader.hpp"
#include "../Matrix/MatrixDescriptor.hpp"
#include "../Matrix/MatrixComposition.hpp"
#include "../Matrix/M3.hpp"
#include "../Matrix/MatrixParsers.hpp"
#include "../Builder/Builder.hpp"
#include "../Counter/ICounter.hpp"
#include "../ObjectFactory/Factory.hpp"
#include "../Common/DateTimes.hpp"
#include "../CSV/Elements.hpp"
#include "../CSV/Matcher.hpp"
#include "../Quantity/Quantity.hpp"
#include "../Functional/Functional.hpp"
#include "../Common/DateTimes.hpp"
#include "../Visitor/CollectorVisitor.hpp"

#pragma once

template<typename Q>
class ICalculator
{
	inline static constexpr const char TypeIdentifier[] = "ICalculator";
    inline static constexpr Literal TypeId{TypeIdentifier};
	template<typename U> using IsT =  Is<U,TypeId>;
public:
    inline static constexpr size_t Order = 1;
    using QuantityType = Q;    
    using FuncType = Acc<QuantityType>;    
	using ElementType = std::shared_ptr<IElement>;
	using DescriptorType = MatrixDescriptor<1,ElementType>;
	using MatrixType = Matrix<Order, DescriptorType>;
	MatrixType operator()() const { return exec(); };
    auto Value() const { return value(); };    
private:
	friend std::ostream& operator<<(std::ostream& s, const ICalculator& i) { return i.display(s); }
	virtual std::ostream& display(std::ostream& s) const { return s<<exec(); };
	virtual MatrixType exec() const = 0;
	virtual QuantityType value() const
    {
        auto m = exec();
        auto acc = FuncType();
        for(auto i = 0; i < m.Rows(); ++i)
            acc.Push(m[i].template As<Quantity<Sum>>());
        return QuantityType(acc());
    };
};

template<typename Q>
class CalculatorBase: public ICalculator<Q>
{
	inline static constexpr const char TypeIdentifier[] = "CalculatorBase";
    inline static constexpr Literal TypeId{TypeIdentifier};
	template<typename U> using IsT =  Is<U,TypeId>;
protected:
	CalculatorBase(std::shared_ptr<Factory<IElement>> fE,std::shared_ptr<Factory<BaseVisitor>> fB): elementFactory{fE}, visitorFactory{fB} {};
	std::shared_ptr<Factory<IElement>> elementFactory;
	std::shared_ptr<Factory<BaseVisitor>> visitorFactory;
};
