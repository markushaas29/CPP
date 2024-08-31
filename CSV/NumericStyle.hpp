 #include "../String/Literal.hpp"

#pragma once
 
template<int N, Literal L>
class NumericStyle
{
    //inline static constexpr Literal TypeId{TypeIdentifier};
    //template<typename U> using IsT =  Is<U,TypeId>;
	Literal<L.Size> literal;
	friend std::ostream& operator<<(std::ostream& s, const NumericStyle& i) { return s<<i.literal;  }
public:
	inline static std::string Identifier = "TagDecorator";
};

template<int N>
struct Px: public NumericStyle<N,"Px">				{	virtual std::string decorate() const { return "b"; } };
