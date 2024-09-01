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

template<int N>
struct Px: public NumericStyle<N, Px>				{	inline static std::string Name = "px";  };

template<int N>
struct AreaNum: public NumericStyle<N, AreaNum>				{	inline static std::string Name = "";  };

struct DinA4				{	inline static const std::string Id = "\'0\' \'0\' \'0\' \'0\' \'1\' \n \'0\' \'2\' \'2\' \'2\' \'0\' \n \'0\' \'3\' \'3\' \'3\' \'0\'  \n \'0\' \'4\' \'4\' \'4\' \'0\'";  };
//struct Px: public NumericStyle<N,"Px">				{	virtual std::string decorate() const { return "b"; } };
