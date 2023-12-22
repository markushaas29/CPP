#include <map>
#include <chrono>
#include <memory>
#include <tuple>
#include "../Is/Is.hpp"
#include "../String/Literal.hpp"

#pragma once

template<typename I, typename... T>
class Registration
{
	using Tup = std::tuple<T...>;
public:
	inline static constexpr const char TypeIdentifier[] = "Registration";
	inline static constexpr Literal TypeId{TypeIdentifier};

	Registration() = delete;
	Registration(I* i): interface(i)	{ reg<0>();	}
private:
	std::tuple<T...> t;
	I* interface;
	friend std::ostream& operator<<(std::ostream& s, const Registration& c){return s;}
	 
	template<size_t N>
	void reg()
	{
		if constexpr (std::tuple_size<Tup>()==N)
			return;
		else
		{	//
			std::cout<<std::get<N>(t)<<std::endl;
			interface->Register("Q",[](const std::string& s) { return std::make_unique<std::tuple_element_t<N,Tup>>(s); });
	    reg<N+1>();}
	}
};
