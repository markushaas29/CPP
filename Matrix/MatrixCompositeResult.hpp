#include <memory>
#include <tuple>
#include <vector>
#include "../Is/Is.hpp"
#include "../String/Literal.hpp"
#include "../CSV/Elements.hpp"    

#pragma once

template<std::size_t, typename> class Matrix;

template<typename Q, typename M>
class MatrixCompositeResult
{
public:
	using QuantityType = Q;
	using MatrixType = M;
	MatrixCompositeResult(const QuantityType& q, const MatrixType& m, const std::string& n =""): value{q}, items(m), name{n} {};
	decltype(auto) Value() { return value; }
	decltype(auto) Items() { return items; }
private:
	friend 	std::ostream& operator<<(std::ostream& out, const MatrixCompositeResult& s)
	{ 
		out<<"Name: "<<s.name<<"Units:\n";
		return out<<"\nItems:\n"<<s.items<<"\n\nResult: "<<s.value;	
	}
	QuantityType value;
	MatrixType items;
	std::string name;
};
