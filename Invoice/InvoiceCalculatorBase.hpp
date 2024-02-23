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

class IInvoiceCalculator
{
	inline static constexpr const char TypeIdentifier[] = "IInvoiceCalculator";
    inline static constexpr Literal TypeId{TypeIdentifier};
	template<typename U> using IsT =  Is<U,TypeId>;
public:
    inline static constexpr size_t Order = 1;
    using QuantityType = Quantity<Sum>;    
    using FuncType = Acc<QuantityType>;    
	using ElementType = std::shared_ptr<IElement>;
	using DescriptorType = MatrixDescriptor<1,ElementType>;
	using MatrixType = Matrix<Order, DescriptorType>;
	MatrixType operator()() const { return exec(); };
    auto Value() const { return value(); };    
private:
	friend std::ostream& operator<<(std::ostream& s, const IInvoiceCalculator& i) { return i.display(s); }
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

class InvoiceCalculatorBase: public IInvoiceCalculator
{
	inline static constexpr const char TypeIdentifier[] = "InvoiceCalculatorBase";
    inline static constexpr Literal TypeId{TypeIdentifier};
	template<typename U> using IsT =  Is<U,TypeId>;
protected:
	InvoiceCalculatorBase(std::shared_ptr<Factory<IElement>> fE,std::shared_ptr<Factory<BaseVisitor>> fB): elementFactory{fE}, visitorFactory{fB} {};
	std::shared_ptr<Factory<IElement>> elementFactory;
	std::shared_ptr<Factory<BaseVisitor>> visitorFactory;
};
