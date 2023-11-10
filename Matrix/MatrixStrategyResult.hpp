#include <memory>
#include <tuple>
#include <vector>
#include "../Is/Is.hpp"
#include "../String/Literal.hpp"
#include "../CSV/Elements.hpp"    

#pragma once

template<std::size_t, typename> class Matrix;

template<typename Q, typename M, typename U>
class StrategyResult
{
public:
	using QuantityType = Q;
	using MatrixType = M;
	using UnitType = U;
	StrategyResult(const QuantityType& q, const MatrixType& m, const std::vector<UnitType>& u, const std::string& n =""): result{q}, items(m), units{u}, name{n} {};
	decltype(auto) Result() { return result; }
	decltype(auto) Items() { return items; }
	decltype(auto) Compose(const StrategyResult& r) 
	{ 
		auto n = name + " / " + r.name;
		auto u = units;
		u.insert(std::end(u), std::begin(r.units), std::end(r.units));

		std::vector<typename MatrixType::DataType> res;
		for(auto i = 0; i < items.Rows(); ++i)
		{
			auto row = items.row(i);
			std::for_each(row.cbegin(), row.cend(), [&](const auto& v) { res.push_back(v); });
		}
		for(auto i = 0; i < r.items.Rows(); ++i)
		{
			auto row = items.row(i);
			std::for_each(row.cbegin(), row.cend(), [&](const auto& v) { res.push_back(v); });
		}

		std::array<size_t,MatrixType::Order> e;
		std::copy(items.descriptor.Extents().begin(), items.descriptor.Extents().end(), e.begin());
		e[0] = res.size() / e[1];

		auto m = MatrixType(typename MatrixType::DescriptorType{e}, res); 
		
		return StrategyResult(result+r.result, m,u,n); 
	}
private:
	friend 	std::ostream& operator<<(std::ostream& out, const StrategyResult& s)
	{ 
		out<<"Name: "<<s.name<<"Units:\n";
		std::for_each(s.units.cbegin(), s.units.cend(), [&](auto& u) { out<<u<<"\n"; });
		return out<<"\nItems:\n"<<s.items<<"\n\nResult: "<<s.result;	
	}
	QuantityType result;
	MatrixType items;
	std::vector<UnitType> units;
	std::string name;
};
