 #include <string>
 #include "../String/Literal.hpp"

#pragma once
 


template<int N, template<int> class D>//, Literal L>
class NumericStyle
{
    //inline static constexpr Literal TypeId{TypeIdentifier};
    //template<typename U> using IsT =  Is<U,TypeId>;
	//Literal<L.Size> literal;
	static constexpr int Num = N;
	friend std::ostream& operator<<(std::ostream& s, const NumericStyle& i) { return s<<NumericStyle::Id;  }
public:
	inline static const std::string Id = std::to_string(N) + D<N>::Name;
};

template<template<int> class D, int ...>  
struct NumericStyleExpander;

template<template<int> class D, int I>
struct NumericStyleExpander<D,I>: public NumericStyle<I,D>
{  
	inline static const std::string Id = NumericStyle<I,D>::Id; 
};

template<template<int> class D, int I, int ... Tail> 	
struct NumericStyleExpander<D,I, Tail ...>: public NumericStyle<I,D> 
{  
	inline static const std::string Id = NumericStyle<I,D>::Id + " " + NumericStyleExpander<D,Tail ...>::Id; 
};

template<Literal L>
class Hex				
{	
	inline static Literal l = L;  
	static constexpr decltype(auto) check() 
	{ 
		static_assert(decltype(l)::Size==7);
		return l.Ptr(); 
	};
	static constexpr decltype(auto) checkValues() 
	{ 
		for(uint i = 0; i < decltype(l)::Size; ++i)

		return l.Ptr(); 
	};
public:
	inline static std::string Id = "#" + std::string(check());
};

template<int... N>
struct Px: public NumericStyleExpander<Px, N...>				{	inline static std::string Name = "px";  };

template<int... N>
struct AreaNum: public NumericStyleExpander<AreaNum, N...>				{	inline static std::string Name = "";  };

struct DinA4				{	inline static const std::string Id = "\'0\' \'0\' \'0\' \'0\' \'1\' \n \'0\' \'2\' \'2\' \'2\' \'0\' \n \'0\' \'3\' \'3\' \'3\' \'0\'  \n \'0\' \'4\' \'4\' \'4\' \'0\'";  };
//struct Px: public NumericStyle<N,"Px">				{	virtual std::string decorate() const { return "b"; } };
