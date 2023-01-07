#include <string>
#include <iostream>
#include <cmath>
#include "QuantityRatio.hpp"
#include "../Unit/Unit.hpp"
#include "../Logger/Logger.hpp"
#include "../String/StringParser.hpp"
#include "../String/String_.hpp"

#ifndef QUANTITY_H
#define QUANTITY_H

template<typename U, typename QR = QuantityRatio<1,U>,typename T1 = double>
class Quantity
{
public:
	using ValueType = T1;
	using UnitType = U;
	using QuantityRatioType = QR;
	using Converter = String_::ParserTo<T1>;
	using Type = Quantity<U,QR,T1>;	
	using PureType = Quantity<U,Pure<U>,T1>;	
	
    const std::string UnitSign() { return U::Sign(); }
    const std::string SiUnit() { return UnitType::SiUnit(); }
    inline static const std::string Identifier = U::Name;
    inline static constexpr Converter converter = Converter();
    inline static constexpr String_::CommaToPoint commaToPoint = String_::CommaToPoint();
    
	constexpr Quantity(): value(0 * QR::Factor) {	}
	explicit constexpr Quantity(const T1& v): value(v * QR::Factor) {	}
	explicit Quantity(const std::string& s): value{std::abs(converter(commaToPoint(String_::Remove<String_::Point>(s)))) * QR::Factor} { 	}
	
	constexpr T1 Value() const { return value / QR::Factor;}
	constexpr T1 PureValue() const { return value;}
	constexpr decltype(auto) ToPure() const { return PureType(value);}
	
	template<typename U2 = U, typename SiPrefix2 = QR, typename T2 = T1>
	Quantity(Quantity<U2,SiPrefix2,T2> q ):value(q.Value()){ Logger::Log()<<"CopyValue: "<<value<<std::endl;	}
	
	bool operator<(const Quantity<U,QR,T1>& y) const { return this->Value() < y.Value(); }
	bool operator>(const Quantity<U,QR,T1>& y) const { return this->Value() > y.Value(); }
	bool operator>=(const Quantity<U,QR,T1>& y) const { return this->Value() >= y.Value(); }
	bool operator==(const Quantity<U,QR,T1>& y) const { return this->Value() == y.Value(); }
	bool operator!=(const Quantity<U,QR,T1>& y) const { return !(this->Value() == y.Value()); }
	
	// ----------------------------------------ADD-------------------------------------------------------------
	decltype(auto) operator+(const Quantity<U,QR,T1>& y) const { return Type(Value() +y.Value()); }
	
	template<typename TQR = QR>
	decltype(auto) operator+(const Quantity<U,TQR,T1>& y) const { return *this + transform(y); }
	
	// ----------------------------------------SUB-------------------------------------------------------------
	decltype(auto) operator-(const Quantity<U,QR,T1>& y) const { return Type(Value() - y.Value()); }
	
	template<typename TQR = QR>
	decltype(auto) operator-(const Quantity<U,TQR,T1>& y) const { return *this - transform(y); }
	
	// ----------------------------------------MULTIPLY-------------------------------------------------------------
	
	decltype(auto) operator*(const Quantity<U,QR,T1>& q ) const { return multiply(q);}
	
	template<typename U2 = U, typename TQR = QR>
	decltype(auto) operator*(const Quantity<U2, TQR,T1>& q ) const 
	{
		if constexpr (!IsSameBaseUnit<U,U2>())
			return Quantity<typename Transform< U, U2, MultiplyPolicy>::Type, Pure<U>,T1>(value * q.PureValue());	
		
		return multiply(q);
	}
	
	// ----------------------------------------DIVISION-------------------------------------------------------------
	
	decltype(auto) operator/(const Quantity<U,QR,T1>& q ) const { return Quantity<typename Transform<U, U, DividePolicy>::Type, Pure<U>,T1>(value / q.PureValue());	}
	
	template<typename U2 = U, typename TQR = QR>
	decltype(auto) operator/(const Quantity<U2,TQR,T1>& q ) const { 	return Quantity<typename Transform< U, U2, DividePolicy>::Type, Pure<U>,T1>(value / q.PureValue()); }
	
private:
	T1 value;	
	
	template<typename TQuantity>
	static decltype(auto) transform(TQuantity t){ return Type(t.PureValue() / QR::Factor);}
	
	template<typename U2 = U, typename TQR = QR>
	decltype(auto) multiply(const Quantity<U2, TQR,T1>& q) const
	{ 
		constexpr int ex = QR::Exponent + TQR::Exponent;
		using QR_ = typename QR::template Creator<ex>;
		return Quantity<typename Transform<U, U2, MultiplyPolicy>::Type, QR_,T1>(Value() * q.Value());
	}
};

template<typename U, typename QR = Pure<U>,typename T1 = double>
std::ostream& operator<<(std::ostream& out, const Quantity<U,QR,T1>& q)
{
	//~ if constexpr (std::is_same_v<QR, Days> || std::is_same_v<QR, Hours> || std::is_same_v<QR, Minutes>)
		//~ return out<<q.Value()<<" "<<QR::Sign;
	return out<<q.Value()<<" "<<QR::Sign<<U::Sign();
}

//~ template<typename U,typename SIPrefix<86400,1> QR,typename T1 = double>
//~ std::ostream& operator<<(std::ostream& out, const Quantity<U,QR,T1>& q)
//~ {
	//~ return out<<q.Value()<<" "<<QR::Sign;
//~ }


#endif
