#include <memory>
#include <tuple>
#include <vector>
#include "../Is/Is.hpp"
#include "../String/Literal.hpp"
#include "../CSV/Elements.hpp"    
#include "../Functional/Functional.hpp"    

#pragma once

template<typename Q>
class IInvoice
{
public:
	using QuantityType = Q;
	using FuncType = Acc<Q>;
	virtual Q Value() const { return value(); };
private:
	friend 	std::ostream& operator<<(std::ostream& out, const IInvoice& s) {	return s.display(out);	}
	virtual std::ostream& display(std::ostream& out)	const = 0;
	virtual Q value() const = 0;
};

template<typename T>
class Invoice: public IInvoice<Quantity<Sum>>
{
	using Base = IInvoice<Quantity<Sum>>;
public:
//	Invoice(const Q&& q, const MType&& m = MType(), const std::string& n =""): value{q}, item(m), name{n} {};
private:
	friend 	std::ostream& operator<<(std::ostream& out, const Invoice& s)	{	return out<<"Result: "<<s.result;	}
	std::ostream& display(std::ostream& out) const { return out<<(*this); }
	virtual typename Base::QuantityType value() const { return result; }
	typename Base::QuantityType result;
	std::string name;
};
