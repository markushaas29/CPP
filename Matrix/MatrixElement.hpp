#include <memory>
#include "../Is/Is.hpp"
#include "../CSV/Element.hpp"
#include "../CSV/Elements.hpp"
#include "../Calculator/CalculatorResult.hpp"
#include "../String/Literal.hpp"
#include "../Quantity/Quantity.hpp"
#include "../Quantity/QuantityRatio.hpp"

#pragma once

class IMatrixElement
{
public:
	virtual std::unique_ptr<IMatrixElement> Clone() = 0;
	virtual std::ostream& Display(std::ostream& s) const = 0;
};

std::ostream& operator<<(std::ostream& out, const IMatrixElement& e) {	return e.Display(out); }

template<typename T> class MatrixElement;

template<typename T, typename ET>
class MatrixElementBase: public IMatrixElement
{
public:
	virtual ~MatrixElementBase() {  }
	using Type = MatrixElement<T>;
	using ValueType = T;
	using ElementType = ET;
	using PtrType = std::unique_ptr<ElementType>;
	template<typename O>
	operator O() const { return static_cast<O>(value); }
	template<typename O>
	decltype(auto) To() const 
	{ 

		if constexpr (!std::is_same_v<ValueType,std::string>)
			return static_cast<O>(value);
		else
		{
			std::string s(value);
			std::replace(s.begin(), s.end(), ',', '.');
			std::istringstream iss(static_cast<std::string>(s));
			O val;
			iss>>val;
			return val;
		}
	}
	decltype(auto) operator()() const 
	{
		if constexpr (	IsResultType<ValueType>	)
			return static_cast<double>(value);
		else
			return static_cast<ValueType>(value); 
	}
	std::unique_ptr<IMatrixElement> Clone() { return std::make_unique<Type>(*this); }
	decltype(auto) Get() const { return ElementType((*this)()); }
	const ValueType& Value() const { return value; }
protected:
	inline static constexpr const char TypeIdentifier[] = "MatrixElement";
	inline static constexpr Literal LiteralType{TypeIdentifier};
	MatrixElementBase(const ValueType& v): value{v} {  }
private:
	friend Type;
	ValueType value;
	friend std::ostream& operator<<(std::ostream& s, const MatrixElementBase& me) { return me.Display(s);  }
};

template<typename T> class MatrixElement{ };

template<typename T> requires ( std::is_arithmetic_v<T> )
class MatrixElement<T>: public MatrixElementBase<T, Quantity<Scalar,Pure,T>>
{
public:
	using Base = MatrixElementBase<T, Quantity<Scalar,Pure,T>>;
	MatrixElement(T v): Base{v} { }
	std::ostream& Display(std::ostream& os) const { return os<<Base::LiteralType<<": "<<Base::value; }
private:
	friend std::ostream& operator<<(std::ostream& s, const MatrixElement& me) { return me.Display(s);  }
};

template<> 
class MatrixElement<std::string>: public MatrixElementBase<std::string,Entry>
{
public:
	using Base = MatrixElementBase<std::string, Entry>;
	MatrixElement(const std::string& v): Base{v} { }
	std::ostream& Display(std::ostream& os) const { return os<<Base::LiteralType<<": "<<Base::value; }
private:
	friend std::ostream& operator<<(std::ostream& s, const MatrixElement& me) { return me.Display(s);  }
};

template<typename T> requires ( IsResultType<T> )
class MatrixElement<T>: public MatrixElementBase<T, Quantity<Scalar,Pure,double>>
{
public:
	using Base = MatrixElementBase<T, Quantity<Scalar,Pure,double>>;
	MatrixElement(const T& v): Base{v} { }
	std::ostream& Display(std::ostream& os) const { return os<<Base::LiteralType<<" :"<<Base::value; }
private:
	friend std::ostream& operator<<(std::ostream& s, const MatrixElement& me) { return me.Display(s);  }
};
