#include <string>
#include <iostream>
#include <cmath>
#include "../Unit/Unit.hpp"
#include "../Logger/Logger.hpp"
#include "../Unit/SIPrefix.hpp"
#include "../String/StringParser.hpp"
#include "../String/String_.hpp"
// #include "../Wrapper/Wrapper.hpp"

#ifndef QUANTITY_H
#define QUANTITY_H

template<typename U, typename SiPrefix = SIPrefix<1,1>,typename T1 = double>
class Quantity
{
public:
	using ValueType = T1;
	using UnitType = U;
	using UnitPrefix = SiPrefix;
	using Converter = String_::ParserTo<T1>;
	using Type = Quantity<U,SiPrefix,T1>;	
	using PureType = Quantity<U,Pure,T1>;	
	
    const std::string UnitSign() { return U::Sign(); }
    const std::string SiUnit() { return UnitType::SiUnit(); }
    inline static const std::string Identifier = U::Name;
    inline static constexpr Converter converter = Converter();
    inline static constexpr String_::CommaToPoint commaToPoint = String_::CommaToPoint();
    
	constexpr Quantity(): value(0 * SiPrefix::Factor) {	}
	explicit constexpr Quantity(const T1& v): value(v * SiPrefix::Factor) {	}
	explicit Quantity(const std::string& s): value{std::abs(converter(commaToPoint(String_::Remove<String_::Point>(s))))} { /*Logger::Log()<<"Value: "<<value<<std::endl;*/	}
	
	T1 Value() const { return value / SiPrefix::Factor;}
	T1 PureValue() const { return value;}
	constexpr decltype(auto) ToPure() const { return PureType(value);}
	
	template<typename U2 = U, typename SiPrefix2 = SiPrefix, typename T2 = T1>
	Quantity(Quantity<U2,SiPrefix2,T2> q ):value(q.Value()){ Logger::Log()<<"CopyValue: "<<value<<std::endl;	}
	
	bool operator<(const Quantity<U,SiPrefix,T1>& y) const { return this->Value() < y.Value(); }
	bool operator>(const Quantity<U,SiPrefix,T1>& y) const { return this->Value() > y.Value(); }
	bool operator>=(const Quantity<U,SiPrefix,T1>& y) const { return this->Value() >= y.Value(); }
	bool operator==(const Quantity<U,SiPrefix,T1>& y) const { return this->Value() == y.Value(); }
	bool operator!=(const Quantity<U,SiPrefix,T1>& y) const { return !(this->Value() == y.Value()); }
	
	// ----------------------------------------ADD-------------------------------------------------------------
	decltype(auto) operator+(const Quantity<U,SiPrefix,T1>& y) const { return Quantity<U,Pure,T1>(value + y.PureValue()); }
	
	template<typename SiPrefix2 = SiPrefix>
	decltype(auto) operator+(const Quantity<U,SiPrefix2,T1>& y) const { return Quantity<U,Pure,T1>(value + y.PureValue()); }
	
	// ----------------------------------------SUB-------------------------------------------------------------
	decltype(auto) operator-(const Quantity<U,SiPrefix,T1>& y) const { return Quantity<U,Pure,T1>(value - y.PureValue()); }
	
	template<typename SiPrefix2 = SiPrefix>
	decltype(auto) operator-(const Quantity<U,SiPrefix2,T1>& y) const { return Quantity<U,Pure,T1>(value - y.PureValue()); }
	
	// ----------------------------------------MULTIPLY-------------------------------------------------------------
	
	decltype(auto) operator*(const Quantity<U,SiPrefix,T1>& q ) const { return Quantity<typename Transform<U, U, MultiplyPolicy>::Type, Pure,T1>(value * q.PureValue());}
	
	template<typename U2 = U, typename SiPrefix2 = SiPrefix>
	decltype(auto) operator*(const Quantity<U2, SiPrefix2,T1>& q ) const { 	return Quantity<typename Transform< U, U2, MultiplyPolicy>::Type, Pure,T1>(value * q.PureValue());	}
	
	// ----------------------------------------DIVISION-------------------------------------------------------------
	
	decltype(auto) operator/(const Quantity<U,SiPrefix,T1>& q ) const { return Quantity<typename Transform<U, U, DividePolicy>::Type, Pure,T1>(value / q.PureValue());	}
	
	template<typename U2 = U, typename SiPrefix2 = SiPrefix>
	decltype(auto) operator/(const Quantity<U2,SiPrefix2,T1>& q ) const { 	return Quantity<typename Transform< U, U2, DividePolicy>::Type, Pure,T1>(value / q.PureValue()); }
	
private:
	T1 value;	
};

template<typename U, typename SiPrefix = SIPrefix<1,1>,typename T1 = double>
std::ostream& operator<<(std::ostream& out, const Quantity<U,SiPrefix,T1>& q)
{
	if constexpr (std::is_same_v<SiPrefix, Days> || std::is_same_v<SiPrefix, Hours> || std::is_same_v<SiPrefix, Minutes>)
		return out<<q.Value()<<" "<<SiPrefix::Sign;
	return out<<q.Value()<<" "<<SiPrefix::Sign<<U::Sign();
}

//~ template<typename U,typename SIPrefix<86400,1> SiPrefix,typename T1 = double>
//~ std::ostream& operator<<(std::ostream& out, const Quantity<U,SiPrefix,T1>& q)
//~ {
	//~ return out<<q.Value()<<" "<<SiPrefix::Sign;
//~ }


#endif
