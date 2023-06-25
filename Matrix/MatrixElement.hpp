#include <memory>
#include "../Is/Is.hpp"
#include "../CSV/Element.hpp"
#include "../String/Literal.hpp"
#include "../String/To/To.hpp"
#include "../Quantity/Quantity.hpp"
#include "../Quantity/QuantityRatio.hpp"

#pragma once

class IMatrixElement
{
public:
	virtual std::unique_ptr<IMatrixElement> Clone() = 0;
	virtual std::ostream& Display(std::ostream& s) const = 0;
};

class MatrixElementBase: public IMatrixElement
{
public:
	virtual ~MatrixElementBase() {  }
protected:
	inline static constexpr const char TypeIdentifier[] = "MatrixElement";
	inline static constexpr Literal LiteralType{TypeIdentifier};
	MatrixElementBase() {  }
private:
	friend std::ostream& operator<<(std::ostream& s, const MatrixElementBase& me) { return me.Display(s);  }
};

template<typename T, typename D>
class MatrixElement: public MatrixElementBase
//class MatrixElement: public IMatrixElement
{
public:
	using PtrType = std::unique_ptr<Element<T>>;
	using Derived = D;
	MatrixElement(const MatrixElement& m): element{std::make_unique<T>((element->Value()).c_str())} {  }
	decltype(auto) Get() const { return T{element->Value()}; }
	decltype(auto) Cast() const { return static_cast<const Derived*>(this); }
	PtrType Ptr() const { return std::make_unique<T>((element->Value()).c_str()); }
	std::unique_ptr<IMatrixElement> Clone() { return std::make_unique<MatrixElement<T,D>>(*this); }
	std::ostream& Display(std::ostream& os) const { return os<<LiteralType<<" :"<<*element; }
protected:
	MatrixElement(PtrType p): element{std::move(p)} {  }
private:
	friend std::ostream& operator<<(std::ostream& s, const MatrixElement& me) { return Display(s);  }
	PtrType element;
};

template<typename V>
class ValueElement: public MatrixElement<V,ValueElement<V>>
{
	using Type = V;
	using Base = MatrixElement<V,ValueElement<V>>;
	inline static constexpr const char TypeIdentifier[] = "ValueElement";
	inline static constexpr Literal LiteralType{TypeIdentifier};
public:
	ValueElement(std::string v): Base{std::make_unique<V>(v.c_str())}, value{v} {};
	std::unique_ptr<IMatrixElement> Clone() { return std::make_unique<ValueElement<V>>(*this); }
	std::ostream& Display(std::ostream& os) const { return os<<LiteralType; }
private:
	V value;
	friend std::ostream& operator<<(std::ostream& s, const ValueElement& vc) { return s<<vc.value;  }
};

template<typename QT>
class QuantityElement: public MatrixElement<QT,QuantityElement<QT>>
{
	using Type = QT;
	using Base = MatrixElement<QT,QuantityElement<QT>>;
	inline static constexpr const char TypeIdentifier[] = "QElement";
	inline static constexpr Literal LiteralType{TypeIdentifier};
public:
	QuantityElement(double v): quantity{v} {};
	QuantityElement(std::string s): Base{std::make_unique<QT>(s.c_str())}, quantity{String_::ParseTo<double>(s)} {  }
	std::unique_ptr<IMatrixElement> Clone() { return std::make_unique<QuantityElement<Type>>(*this); }
	std::ostream& Display(std::ostream& os) const { return os<<LiteralType; }
private:
	friend std::ostream& operator<<(std::ostream& s, const QuantityElement& me) { return s<<me.quantity<<"..."<<me.Base::Get();  }
	Type quantity;
};
