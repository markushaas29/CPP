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
	inline static std::string Id = std::to_string(N) + D<N>::Name;
};

template<int N>
struct Px: public NumericStyle<N, Px>				{	inline static std::string Name = "px";  };
//struct Px: public NumericStyle<N,"Px">				{	virtual std::string decorate() const { return "b"; } };
