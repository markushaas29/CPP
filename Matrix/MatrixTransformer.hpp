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

template<typename... T>
class MatrixTransformer 
{
public:
	using Tuple = std::tuple<T...>;
	static int constexpr Size = std::tuple_size_v<Tuple>;
	using InputIterator = std::vector<std::string>::const_iterator;
	auto Create() 
	{
		std::vector<std::string> s{"1","2","27.12.2022"};
		Tuple t{};
		std::cout<<"S: "<<Size<<std::endl;
		return createIntern<0>(t,s.cbegin(),s.cend());
		//return t;
	} 	
private:
	template<int N>
	auto createIntern(auto& t, InputIterator begin, InputIterator end) 
	{
		if constexpr (N==Size)
			return t;
		else
		{
			using Type = std::tuple_element_t<N, Tuple>;
			auto s = begin + N;
			//std::get<N>(t)=std::move(Type{*s});
			if constexpr (N==0)
			{
				auto tN =  std::make_tuple(Type{*s});
				return createIntern<N+1>(tN,begin,end);
			}
			else
			{
				auto tN = std::tuple_cat(t,std::tuple<Type>{*s});
				return createIntern<N+1>(tN,begin,end);
			}
		}
	} 	
	friend std::ostream& operator<<(std::ostream& s, const MatrixTransformer& me) { return s;  }
};
