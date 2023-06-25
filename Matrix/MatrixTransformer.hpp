#include <memory>
#include <tuple>
#include <vector>
#include "../Is/Is.hpp"
#include "MatrixElement.hpp"
#include "../String/Literal.hpp"
#include "../String/To/To.hpp"
#include "../Quantity/Quantity.hpp"
#include "../CSV/Elements.hpp"    
#include "../Common/DateTimes.hpp"
#pragma once

template<typename T=int>
class MatrixTransformer 
{
public:
	using Tuple = std::tuple<Quantity<Sum>,IBAN,BIC>;//DateTimes::Date>;
	static int constexpr Size = std::tuple_size_v<Tuple>;
	using InputIterator = std::vector<std::string>::const_iterator;
	auto Create() 
	{
		std::vector<std::string> s{"1","2","3"};
		Tuple t{};
		std::cout<<"S: "<<Size<<std::endl;
		return createIntern<0>(t,s.cbegin(),s.cend());
		//return t;
	} 	
private:
	template<int N>
	auto createIntern(Tuple& t, InputIterator begin, InputIterator end) 
	{
		if constexpr (N==Size)
			return t;
		else
		{
			using Type = std::tuple_element_t<N, Tuple>;
			auto s = begin + N;
			std::get<N>(t)=Type{*s};
			return createIntern<N+1>(t,begin,end);
		}
	} 	
	friend std::ostream& operator<<(std::ostream& s, const MatrixTransformer& me) { return s;  }
};
