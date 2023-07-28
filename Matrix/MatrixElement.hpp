#include <memory>
#include "../Is/Is.hpp"
#include "../CSV/Element.hpp"
#include "../CSV/Elements.hpp"
#include "../Calculator/CalculatorResult.hpp"
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

template<typename T>
class MatrixElement: public MatrixElementBase
{
public:
	using PtrType = std::unique_ptr<Element<T>>;
	using Type = T;
	MatrixElement(const std::string& v): element{T(v.c_str())} { }
	template<typename I>
	MatrixElement(I v): element{T(v)} { }
	
	decltype(auto) Get() const { return T(element.Value()); }
	PtrType Ptr() const { return std::make_unique<T>((element.Value()).c_str()); }
	
	std::unique_ptr<IMatrixElement> Clone() { return std::make_unique<MatrixElement<T>>(*this); }
	std::ostream& Display(std::ostream& os) const { return os<<LiteralType<<" :"<<element; }
protected:
	MatrixElement(PtrType p): element{std::move(p)} {  }
private:
	friend std::ostream& operator<<(std::ostream& s, const MatrixElement& me) { return me.Display(s);  }
	Type element;
};

template<typename T> requires ( std::is_arithmetic_v<T> )
class MatrixElement<T>: public MatrixElementBase
{
public:
	using PtrType = std::unique_ptr<Element<T>>;
	using Type = T;
	MatrixElement(const std::string& v): element{T(v.c_str())} { }
	template<typename I>
	MatrixElement(I v): element{T(v)} { }
	
	decltype(auto) Get() const { return T(element.Value()); }
	PtrType Ptr() const { return std::make_unique<T>((element.Value()).c_str()); }
	
	std::unique_ptr<IMatrixElement> Clone() { return std::make_unique<MatrixElement<T>>(*this); }
	std::ostream& Display(std::ostream& os) const { return os<<LiteralType<<" AR :"<<element; }
protected:
	MatrixElement(PtrType p): element{std::move(p)} {  }
private:
	friend std::ostream& operator<<(std::ostream& s, const MatrixElement& me) { return me.Display(s);  }
	Type element;
};
template<> 
class MatrixElement<std::string>: public MatrixElementBase
{
public:
	template<typename I>
	MatrixElement(I v) { }
	using PtrType = std::unique_ptr<Element<Entry>>;
	std::unique_ptr<IMatrixElement> Clone() { return std::make_unique<MatrixElement<std::string>>(*this); }
	std::ostream& Display(std::ostream& os) const { return os<<LiteralType<<" STRINg :"; }
protected:
private:
	friend std::ostream& operator<<(std::ostream& s, const MatrixElement& me) { return me.Display(s);  }
};
template<typename T> requires ( IsResultType<T> )
class MatrixElement<T>: public MatrixElementBase
{
public:
	using PtrType = std::unique_ptr<Element<T>>;
	using Type = T;
	MatrixElement(const std::string& v): element{T(v.c_str())} { }
	template<typename I>
	MatrixElement(I v): element{T(v)} { }
	
	decltype(auto) Get() const { return T(element.Value()); }
	PtrType Ptr() const { return std::make_unique<T>((element.Value()).c_str()); }
	
	std::unique_ptr<IMatrixElement> Clone() { return std::make_unique<MatrixElement<T>>(*this); }
	std::ostream& Display(std::ostream& os) const { return os<<LiteralType<<" RESULT :"<<element; }
protected:
	MatrixElement(PtrType p): element{std::move(p)} {  }
private:
	friend std::ostream& operator<<(std::ostream& s, const MatrixElement& me) { return me.Display(s);  }
	Type element;
};

template<typename I, typename E> 
decltype(auto) CreateElement(I s) { return MatrixElement<E>(s); }

template<typename S> 
decltype(auto) ParseElement(S s) 
{
	if constexpr (std::is_same_v<S,std::string>)
		return MatrixElement<Entry>{s}; 
	if constexpr (std::is_arithmetic_v<S>)
		return MatrixElement<Quantity<Scalar,Pure,S>>(s); 
}

template<typename S> 
decltype(auto) ParseElement(std::shared_ptr<S> s) { return ParseElement(*s); }
//
//template<typename V>
//class ValueElement: public MatrixElement<V,ValueElement<V>>
//{
//	using Type = V;
//	using Base = MatrixElement<V>;
//	inline static constexpr const char TypeIdentifier[] = "ValueElement";
//	inline static constexpr Literal LiteralType{TypeIdentifier};
//public:
//	ValueElement(std::string v): Base{std::make_unique<V>(v.c_str())}, value{v} {};
//	std::unique_ptr<IMatrixElement> Clone() { return std::make_unique<ValueElement<V>>(*this); }
//	std::ostream& Display(std::ostream& os) const { return os<<LiteralType; }
//private:
//	V value;
//	friend std::ostream& operator<<(std::ostream& s, const ValueElement& vc) { return s<<vc.value;  }
//};
//
//template<typename QT>
//class QuantityElement: public MatrixElement<QT,QuantityElement<QT>>
//{
//	using Type = QT;
//	using Base = MatrixElement<QT,QuantityElement<QT>>;
//	inline static constexpr const char TypeIdentifier[] = "QElement";
//	inline static constexpr Literal LiteralType{TypeIdentifier};
//public:
//	QuantityElement(double v): quantity{v} {};
//	QuantityElement(std::string s): Base{std::make_unique<QT>(s.c_str())}, quantity{String_::ParseTo<double>(s)} {  }
//	std::unique_ptr<IMatrixElement> Clone() { return std::make_unique<QuantityElement<Type>>(*this); }
//	std::ostream& Display(std::ostream& os) const { return os<<LiteralType; }
//private:
//	friend std::ostream& operator<<(std::ostream& s, const QuantityElement& me) { return s<<me.quantity<<"..."<<me.Base::Get();  }
//	Type quantity;
//};
