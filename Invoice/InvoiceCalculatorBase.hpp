#include <iostream>
#include <cassert> 
#include <vector> 
#include <map> 
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
#include "../HTMLBuilder/HTMLBuilder.hpp"
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
	MatrixType operator()(const Year& y, const HtmlBuilder<German>& f = HtmlBuilder<German>("")) { return get(y,f); };
	auto Accumulate(size_t b, size_t e) { return acc()(b,e); };
    auto Value(const Year& y, const HtmlBuilder<German>& f = HtmlBuilder<German>("") ) { return value(y,f); };    
private:
	friend std::ostream& operator<<(std::ostream& s, const ICalculator& i) { return i.display(s); }
	virtual std::ostream& display(std::ostream& s) const = 0;
	virtual MatrixType get(const Year& y, const HtmlBuilder<German>& f) = 0;
	virtual QuantityType value(const Year& y, const HtmlBuilder<German>& f)  { return acc(y);    };
	auto acc(const Year& y) 
    {
		HtmlBuilder<German> b("");
        auto m = get(y,b);
        auto acc = FuncType();
        for(auto i = 0; i < m.Rows(); ++i)
            acc.Push(m[i].template As<Quantity<Sum>>());
        return acc;
    };
};

template<typename Q, typename D>
class CalculatorBase: public ICalculator<Q>
{
	inline static constexpr const char TypeIdentifier[] = "CalculatorBase";
    inline static constexpr Literal TypeId{TypeIdentifier};
	template<typename U> using IsT =  Is<U,TypeId>;
	using Base = ICalculator<Q>;
protected:
	CalculatorBase(std::shared_ptr<Factory<IElement>> fE,std::shared_ptr<Factory<BaseVisitor>> fB): cache{std::make_unique<std::map<Year,typename Base::MatrixType>>()}, elementFactory{fE}, visitorFactory{fB} {};
	std::shared_ptr<Factory<IElement>> elementFactory;
	std::shared_ptr<Factory<BaseVisitor>> visitorFactory;
private:
	std::unique_ptr<typename Base::MatrixType> matrix;
	std::unique_ptr<std::map<Year,typename Base::MatrixType>> cache;
	virtual std::ostream& display(std::ostream& s) const { return s<<*matrix; }
	virtual typename Base::MatrixType exec(const Year& y, const HtmlBuilder<German>& f) = 0;
	virtual typename Base::MatrixType get(const Year& y, const HtmlBuilder<German>& f) 
	{
		if(!cache->contains(y))
			(*cache)[y] = this->exec(y,f);
		return (*cache)[y];
	};
};
