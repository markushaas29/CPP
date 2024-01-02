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
	MatrixCompositeResult(const QuantityType& q, const std::vector<MatrixType>& m = std::vector<MatrixType>(), const std::string& n =""): value{q}, items(m), name{n} {};
	decltype(auto) Value() { return value; }
	decltype(auto) Items() { return items; }
private:
	friend 	std::ostream& operator<<(std::ostream& out, const MatrixCompositeResult& s)
	{ 
		out<<"Name: "<<s.name<<"\n";
		std::for_each(s.items.cbegin(), s.items.cend(),[&out](const auto& m) { out<<m<<"\n"; });
		return out<<"\n\nResult: "<<s.value;	
	}
	QuantityType value;
	std::vector<MatrixType> items;
	std::string name;
};
