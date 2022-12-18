#include <string>
#include <iostream>
#include <cmath>
#include "../Unit/Unit.h"
#include "../Logger/Logger.hpp"
#include "../Unit/SIPrefix.hpp"
#include "../String/StringParser.hpp"
#include "../String/String_.hpp"
// #include "../Wrapper/Wrapper.hpp"

#ifndef QUANTITY_H
#define QUANTITY_H

template<typename U, typename SiPrefix = SIPrefix<0>,typename T1 = double>
struct Quantity
{
	using ValueType = T1;
	using UnitType = U;
	using UnitPrefix = SiPrefix;
	using Converter = String_::ParserTo<T1>;
	using Type = Quantity<U,SiPrefix,T1>;	
	
    const std::string UnitSign() { return U::Sign(); }
    const std::string SiUnit() { return UnitType::SiUnit(); }
    inline static const std::string Identifier = U::Name;
    inline static constexpr Converter converter = Converter();
    inline static constexpr String_::CommaToPoint commaToPoint = String_::CommaToPoint();
    
	Quantity(): value(0 * SiPrefix::Factor) {	}
	explicit Quantity(const T1& v): value(v * SiPrefix::Factor) {	}
	explicit Quantity(const std::string& s): value{std::abs(converter(commaToPoint(String_::Remove<String_::Point>(s))))} { /*Logger::Log()<<"Value: "<<value<<std::endl;*/	}
	
	T1 Value() const { return value / SiPrefix::Factor;}
	T1 PureValue() const { return value;}
	
	template<typename U2 = U, typename SiPrefix2 = SiPrefix, typename T2 = T1>
	Quantity(Quantity<U2,SiPrefix2,T2> q ):value(q.Value()){ Logger::Log()<<"CopyValue: "<<value<<std::endl;	}
	
	bool operator<(const Quantity<U,SiPrefix,T1>& y) const { return this->Value() < y.Value(); }
	bool operator>(const Quantity<U,SiPrefix,T1>& y) const { return this->Value() > y.Value(); }
	bool operator>=(const Quantity<U,SiPrefix,T1>& y) const { return this->Value() >= y.Value(); }
	bool operator==(const Quantity<U,SiPrefix,T1>& y) const { return this->Value() == y.Value(); }
	bool operator!=(const Quantity<U,SiPrefix,T1>& y) const { return !(this->Value() == y.Value()); }
	
	// ----------------------------------------ADD-------------------------------------------------------------
	Quantity<U,SiPrefix,T1> operator+(const Quantity<U,SiPrefix,T1>& y) const { return Quantity<U,SiPrefix,T1>(this->Value() + y.Value()); }
	
	template<typename SiPrefix2 = SiPrefix>
	Quantity<U,SiPrefix,T1> operator+(const Quantity<U,SiPrefix2,T1>& y) const { return Quantity<U,SiPrefix,T1>((this->PureValue() + y.Value())/ SiPrefix::Factor); }
	
	// ----------------------------------------SUB-------------------------------------------------------------
	Quantity<U,SiPrefix,T1> operator-(const Quantity<U,SiPrefix,T1>& y) const { return Quantity<U,SiPrefix,T1>(this->Value() - y.Value()); }
	
	template<typename SiPrefix2 = SiPrefix>
	Quantity<U,SiPrefix,T1> operator-(const Quantity<U,SiPrefix2,T1>& y) const { return Quantity<U,SiPrefix,T1>((this->PureValue() - y.Value())/ SiPrefix::Factor); }
	
	// ----------------------------------------MULTIPLY-------------------------------------------------------------
	
	Quantity<typename Transform<U, U,MultiplyPolicy>::Type, SiPrefix,T1> operator*(const Quantity<U,SiPrefix,T1>& q ) const { 
		return Quantity<typename Transform<U, U, MultiplyPolicy>::Type, SiPrefix,T1>(this->PureValue() * q.Value());
	}
	
	template<typename U2 = U, typename SiPrefix2 = SiPrefix>
	Quantity<typename Transform<U, U2,MultiplyPolicy>::Type, SiPrefix,T1> operator*(const Quantity<U2, SiPrefix2,T1>& q ) const { 
		return Quantity<typename Transform< U, U2, MultiplyPolicy>::Type, SiPrefix,T1>(this->Value() * q.Value());
	}
	
	// ----------------------------------------DIVISION-------------------------------------------------------------
	
	Quantity<typename Transform<U, U,DividePolicy>::Type, SiPrefix,T1> operator/(const Quantity<U,SiPrefix,T1>& q ) const { 
		return Quantity<typename Transform<U, U, DividePolicy>::Type, SiPrefix,T1>(this->Value() / q.Value());
	}
	
	template<typename U2 = U, typename SiPrefix2 = SiPrefix>
	Quantity<typename Transform<U, U2,DividePolicy>::Type, SiPrefix,	T1> operator/(const Quantity<U2,SiPrefix2,T1>& q ) const { 
		return Quantity<typename Transform< U, U2, DividePolicy>::Type, SiPrefix,T1>(this->Value() / q.Value());
	}
	
private:
	T1 value;
	
};

template<typename U, typename SiPrefix = SIPrefix<0>,typename T1 = double>
std::ostream& operator<<(std::ostream& out, const Quantity<U,SiPrefix,T1>& q)
{
	return out<<q.Value()<<" "<<SiPrefix::Sign<<U::Sign();
}


#endif
