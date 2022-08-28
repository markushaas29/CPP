#include <string>
#include <iostream>
#include <cmath>
#include "../Unit/Unit.h"
#include "../Logger/Logger.hpp"
#include "../Unit/SIPrefix.hpp"
// #include "../Wrapper/Wrapper.hpp"

#ifndef QUANTITY_H
#define QUANTITY_H

template<typename U, typename SiPrefix = SIPrefix<0>,typename T1 = double>
struct Quantity
{
	using ValueType = T1;
	using UnitType = U;
	using UnitPrefix = SiPrefix;
	using Converter = String_::To<T1>;
	using Type = Quantity<U,SiPrefix,T1>;	
	
    //~ const std::string UnitName() {  return UnitType::Name; }
    //~ const std::string UnitSymbol() { return UnitType::Symbol;}
    const std::string UnitSign() { return U::Sign(); }
    const std::string SiUnit() { return UnitType::SiUnit(); }
    inline static const std::string Identifier = U::Name;
    inline static constexpr Converter converter = Converter();
    inline static constexpr String_::CommaToPoint commaToPoint = String_::CommaToPoint();
    
	Quantity(): value(0 * SiPrefix::Factor) {	}
	explicit Quantity(const T1& v): value(v * SiPrefix::Factor) {	}
	explicit Quantity(const std::string& s): value{std::abs(converter(commaToPoint(String_::Remove<String_::Point>(s))))} { Logger::Log()<<"Value: "<<value<<std::endl;	}
	
	T1 Value() const { return value / SiPrefix::Factor;}
	T1 PureValue() const { return value;}
	
	template<typename U2 = U, typename SiPrefix2 = SiPrefix, typename T2 = T1>
	Quantity(Quantity<U2,SiPrefix2,T2> q ):value(q.Value()){ Logger::Log()<<"CopyValue: "<<value<<std::endl;	}
	
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
	
// 	template<typename Separator = T::char_<';'>>
// 	std::ostream& Display(std::ostream& out) const
// 	{
// // 		auto output = std::string(std::string(this->Value()) + std::string(Separator::Value) + UnitPrefix::Sign + UnitType::Sign());
// 		out<<this->Value()<<Separator::Value<<UnitPrefix::Sign<<UnitPrefix::Sign<<UnitType::Sign();
// // 		out<<output<<std::endl;
// 		return out;
// 	}
	
// 	template<typename Separator = T::char_<';'>>
// 	std::ostream& Display(std::ostream& out) const
// 	{
// // 		out<<Date<<Separator::Value<<QuantityValue.Display(out)<<std::endl;
// // 		out<<Date<<Separator::Value<<QuantityValue.Value()<<Separator::Value<<U::Sign()<<std::endl;
// 		
// 		return out;
// 	}
private:
	T1 value;
	
};

// template<typename U, typename T = double>
template<typename U, typename SiPrefix = SIPrefix<0>,typename T1 = double>
std::ostream& operator<<(std::ostream& out, const Quantity<U,SiPrefix,T1>& q)
{
	return out<<q.Value()<<" "<<SiPrefix::Sign<<U::Sign();
}


#endif
