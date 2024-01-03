#include <memory>
#include <tuple>
#include <vector>
#include "../Is/Is.hpp"
#include "../String/Literal.hpp"
#include "../CSV/Elements.hpp"    

#pragma once

template<std::size_t, typename> class Matrix;

template<typename Q, typename M>
class ICompositeResult
{
public:
	using QuantityType = Q;
	using MatrixType = M;
	virtual Q Value() const = 0;
};

template<typename Q, typename M>
class MatrixCompositionResult: public ICompositeResult<Q,M>
{
	using Base = ICompositeResult<Q,M>;
public:
	MatrixCompositionResult(const Q&& q, const M&& m = M(), const std::string& n =""): value{q}, item(m), name{n} {};
	virtual Q Value() const { return value; }
private:
	friend 	std::ostream& operator<<(std::ostream& out, const MatrixCompositionResult& s)	{	return out<<"Name: "<<s.name<<"\n"<<s.item<<"\n\nValue: "<<s.value;	}
	typename Base::QuantityType value;
	M item;
	std::string name;
};
