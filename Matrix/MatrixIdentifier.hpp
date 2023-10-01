#include <memory>
#include "../Functional/FunctionalConcept.hpp"
#include "../Is/Is.hpp"
#include "../CSV/Element.hpp"
#include "../CSV/Elements.hpp"
#include "../Calculator/CalculatorResult.hpp"
#include "../String/Literal.hpp"
#include "../String/String_.hpp"
#include "../Quantity/Quantity.hpp"
#include "../Quantity/QuantityRatio.hpp"

#pragma once

template<std::size_t, typename> class Matrix;

class IIdentifier
{
public:
	virtual std::unique_ptr<IIdentifier> Clone() = 0;
	virtual std::ostream& Display(std::ostream& s) const = 0;
};

std::ostream& operator<<(std::ostream& out, const IIdentifier& e) {	return e.Display(out); }

template<typename T> class MatrixIdentifier;
template<size_t, typename> class MatrixDescriptor;

template<typename T>
class IdentifierBase: public IIdentifier
{
public:
	virtual ~IdentifierBase() {  }
	using Type = MatrixIdentifier<T>;
	using ValueType = T;
	using StorageType = std::vector<T>;
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
		{
			if constexpr(std::is_invocable_v<ValueType>)
             	 return value();
          	else
				return static_cast<ValueType>(value); 
		}
	}
	std::unique_ptr<IIdentifier> Clone() { return std::make_unique<Type>(*this); }
	decltype(auto) Get() const { return ElementType((*this)()); }
	const ValueType& Value() const { return value; }
protected:
	inline static constexpr const char TypeIdentifier[] = "Identifier";
	inline static constexpr Literal LiteralType{TypeIdentifier};
	IdentifierBase(const ValueType& v): value{v} {  }
private:
	friend Type;
	ValueType value;
	friend std::ostream& operator<<(std::ostream& s, const IdentifierBase& me) { return me.Display(s);  }
};

template<typename T=std::string> 
class MatrixIdentifier: public IdentifierBase<T>
{
public:
	using Base = IdentifierBase<std::string>;
	MatrixIdentifier(const std::string& v): Base{v} { }
	std::ostream& Display(std::ostream& os) const { return os<<Base::LiteralType<<": "<<Base::value; }
private:
	decltype(auto) split(T values) const 
	{
		if constexpr (std::is_same_v<T,std::string>)
			return String_::Split(values,'_');
		else
			return Base::StorageType{values};

	} 
	friend std::ostream& operator<<(std::ostream& s, const MatrixIdentifier& me) { return me.Display(s);  }
};

