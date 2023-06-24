#include <memory>
#include "../Is/Is.hpp"
#include "../CSV/Element.hpp"
#include "../String/Literal.hpp"
#include "../String/To/To.hpp"
#include "../Quantity/Quantity.hpp"
#include "../Quantity/QuantityRatio.hpp"

#pragma once

class IMatrixCell
{
public:
	virtual std::unique_ptr<IMatrixCell> Clone() = 0;
	virtual std::ostream& Display(std::ostream& s) = 0;
};

template<typename T>
class MatrixElement: public IMatrixCell
{
public:
	using PtrType = std::unique_ptr<Element<T>>;
	decltype(auto) getT() const { return T(element->Value()); }
protected:
	MatrixElement(PtrType p): element{std::move(p)} {  }
	MatrixElement(const MatrixElement& m): element{std::make_unique<Element<T>>((element->Value()).c_str())} {  }
	decltype(auto) get() const { return std::make_unique<Element<T>>((element->Value()).c_str()); }
private:
	friend std::ostream& operator<<(std::ostream& s, const MatrixElement& me) { return s<<*me;  }
	PtrType element;
};


template<typename V>
class ValueElement: public MatrixElement<V>
{
	inline static constexpr const char TypeIdentifier[] = "ValueElement";
	inline static constexpr Literal LiteralType{TypeIdentifier};
public:
	//ValueElement(V v): MatrixElement<V>{std::make_unique<V>("Value")} {};
	ValueElement(std::string v): MatrixElement<V>{std::make_unique<V>("Value")} {};
	std::unique_ptr<IMatrixCell> Clone() { return std::make_unique<ValueElement<V>>(*this); }
	std::ostream& Display(std::ostream& os) { return os<<LiteralType; }
	V value;
private:
	friend std::ostream& operator<<(std::ostream& s, const ValueElement& vc) { return s<<vc.value;  }
};

//template<typename UT, typename QR=Pure,typename T1=double>
template<typename QT>
class QuantityElement: public MatrixElement<QT>
{
	using Type = QT;
	using Base = MatrixElement<QT>;
	inline static constexpr const char TypeIdentifier[] = "QCell";
	inline static constexpr Literal LiteralType{TypeIdentifier};
public:
	QuantityElement(double v): quantity{v} {};
	QuantityElement(std::string s): MatrixElement<QT>{std::make_unique<Element<QT>>(s.c_str())}, quantity{String_::ParseTo<double>(s)} {  }
	std::unique_ptr<IMatrixCell> Clone() { return std::make_unique<QuantityElement<Type>>(*this); }
	std::ostream& Display(std::ostream& os) { return os<<LiteralType; }
private:
	friend std::ostream& operator<<(std::ostream& s, const QuantityElement& me) { return s<<me.quantity<<*me.Base::get();  }
	Type quantity;
};
