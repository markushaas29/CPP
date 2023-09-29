#include <string>
#include <iostream>
#include <cmath>
#include <sstream>
#include "QuantityRatio.hpp"
#include "../Unit/Unit.hpp"
#include "../Logger/Logger.hpp"
#include "../String/StringParser.hpp"
#include "../Common/Make/Make.hpp"
#include "../String/String_.hpp"
#include "../To/To.hpp"

#ifndef QUANTITY_H
#define QUANTITY_H

template<typename> class Element;

template<typename U, typename QR = Pure,typename T1 = double>
class Quantity: public Element<Quantity<U,QR,T1>>
{
public:
	using Type = Quantity<U,QR,T1>;	
	using ValueType = T1;
	using UnitType = U;
	using Base = Element<Type>;
	using QuantityRatioType = QR;
	using PureType = Quantity<U,Pure,T1>;	
	
    const std::string UnitSign() const { return U::Sign(); }
	const std::string SiUnit() const { return UnitType::SiUnit(); }
    inline static const std::string Identifier = U::Name;
    inline static constexpr String_::CommaToPoint commaToPoint = String_::CommaToPoint();
    
	constexpr Quantity(): Base("0"), value(0 * QR::Factor) {	}
	explicit constexpr Quantity(const T1& v): Base(""), value(v * QR::Factor) {	}
	explicit Quantity(const std::string& s): Base(s.c_str()), value{(To<ValueType>(s)) * (ValueType)QR::Factor} { 	}
	
	constexpr T1 Value() const { return value / QR::Factor;}
	constexpr T1 PureValue() const { return value;}
	constexpr decltype(auto) ToPure() const { return PureType(value);}
	
	template<typename U2 = U, typename SiPrefix2 = QR, typename T2 = T1>
	constexpr Quantity(Quantity<U2,SiPrefix2,T2> q ):value(q.Value()){ Logger::Log()<<"CopyValue: "<<value<<std::endl;	}
	
	constexpr decltype(auto) operator<=>(const Quantity<U,QR,T1>& y) const { return this->Value() <=> y.Value(); }
	constexpr bool operator==(const Quantity<U,QR,T1>& y) const { return this->Value() == y.Value(); }
	constexpr operator ValueType() const { return Value(); }

	static decltype(auto) Create(std::istream& is)
	{
		ValueType val;
		is>>val;
		return Type{val};
	}
	// ----------------------------------------ADD-------------------------------------------------------------
	constexpr decltype(auto) operator+(const Quantity<U,QR,T1>& y) const { return Type(Value() +y.Value()); }
	
	template<typename TQR = QR>
	constexpr decltype(auto) operator+(const Quantity<U,TQR,T1>& y) const { return Type(Value() + transform(y).Value()); }
	
	// ----------------------------------------SUB-------------------------------------------------------------
	constexpr  decltype(auto) operator-(const Quantity<U,QR,T1>& y) const { return Type(Value() - y.Value()); }
	
	template<typename TQR = QR>
	constexpr decltype(auto) operator-(const Quantity<U,TQR,T1>& y) const { return Type(Value() - transform(y).Value()); }
	
	// ----------------------------------------MOD-------------------------------------------------------------
	constexpr decltype(auto) operator%(const Quantity<U,QR,T1>& y) const { return Type(Value() % y.Value()); }
	
	template<typename TQR = QR>
	constexpr decltype(auto) operator%(const Quantity<U,TQR,T1>& y) const { return Type(Value() % transform(y).Value()); }
	
	// ----------------------------------------MULTIPLY-------------------------------------------------------------
	constexpr decltype(auto) operator*(const Quantity<U,QR,T1>& q ) const { return multiply(q);}
	
	template<typename TQR>
	constexpr decltype(auto) operator*(const Quantity<U,TQR,T1>& q ) const { return multiply(transform(q));}
	
	template<typename U2 = U, typename TQR = QR, typename T2>
	constexpr decltype(auto) operator*(const Quantity<U2, TQR,T2>& q ) const {	return Quantity<typename Transform<U, U2, MultiplyPolicy>::Type, QR,T1>(Value() * (T1)q.PureValue());;	}
	
	// ----------------------------------------DIVISION-------------------------------------------------------------
	constexpr decltype(auto) operator/(const Quantity<U,QR,T1>& q ) const { return Quantity<Scalar>(value / q.PureValue());	}

	template<typename TQR>
	constexpr decltype(auto) operator/(const Quantity<U,TQR,T1>& q ) const { return Quantity<Scalar>( value / transform(q).PureValue());}
	
	template<typename U2 = U, typename TQR = QR, typename T2>
	constexpr decltype(auto) operator/(const Quantity<U2,TQR,T2>& q ) const {	return divide(q);	}
private:
	T1 value;
	friend class Element<Type>;
	static constexpr const char* check(const char* s) { return s; }
	friend std::istream& operator>>(std::istream& s, Quantity& q) 
	{
		ValueType v;
		s>>v;
		q = Quantity{v};
		return s; 
	}
	
	template<typename TQuantity>
	constexpr static decltype(auto) transform(TQuantity t)
	{ 
		using TU = typename TQuantity::UnitType;
		using TQR = typename QR::RatioType<TQuantity::QuantityRatioType::Exponent>;
		return Quantity<TU,TQR>(t.PureValue() / TQR::Factor);
	}
	
	template<typename U2 = U, typename TQR = QR, typename T2>
	constexpr decltype(auto) multiply(const Quantity<U2, TQR,T2>& q) const
	{ 
		constexpr int ex = QR::Exponent + TQR::Exponent;
		using QR_ = typename QR::PowBy<ex>::Type;
		return Quantity<typename Transform<U, U2, MultiplyPolicy>::Type, QR_,T1>(Value() * q.Value());
	}
	
	template<typename U2 = U, typename TQR = QR, typename T2>
	constexpr decltype(auto) divide(const Quantity<U2, TQR,T2>& q) const
	{ 
		constexpr int ex = QR::Exponent - TQR::Exponent;
		using QR_ = typename QR::PowBy<ex>::Type;
		
		if constexpr (TQR::BaseNum == QuantityRatioType::BaseNum && TQR::BaseDenom == QuantityRatioType::BaseDenom )
		{
			if constexpr (IsSameBaseUnit<U,U2>())
				return Quantity<typename Transform<U, U2, DividePolicy>::Type, QR_,T1>(Value() / q.Value());
			
			return Quantity<typename Transform<U, U2, DividePolicy>::Type, QR_,T1>(Value() / q.Value());
		}
		
		if constexpr (IsSameBaseUnit<U,U2>())
			return Quantity<typename Transform<U, U2, DividePolicy>::Type, QR_,T1>(Value() / transform(q).Value());

		return Quantity<typename Transform<U, U2, DividePolicy>::Type, QR_,T1>(Value() / q.PureValue());
	}
};

template<typename U, typename QR,typename T1>
class TryMake<Quantity<U,QR,T1>>
{
public:
	decltype(auto) operator()(std::istream& arg)
	{
		using Target = Quantity<U,QR,T1>;
		auto result = Target::Create(arg);
//		if(!result.Valid())
//			return ParseResult<Target>();
		return ParseResult<Target>(result);
	}
};

template<typename U, typename QR,typename T1>
class Make<Quantity<U,QR,T1>>
{
public:
	decltype(auto) operator()(std::istream& is)
	{
		auto result = TryMake<Quantity<U,QR,T1>>(is);
		if(!result.Valid)
			throw std::runtime_error("Make() failed");
		return result;
	}
};

template<typename U, typename QR = Pure,typename T1 = double>
std::ostream& operator<<(std::ostream& out, const Quantity<U,QR,T1>& q)
{
	//~ if constexpr (std::is_same_v<QR, Days> || std::is_same_v<QR, Hours> || std::is_same_v<QR, Minutes>)
		//~ return out<<q.Value()<<" "<<QR::Sign;
	if constexpr (std::is_same_v<U, Sum>)
	{
		std::ostringstream oss;
		oss << std::setprecision(2)<<std::fixed << q.Value();
		return out<<oss.str()<<" "<<QR::Sign<<U::Sign();
	}
	return out<<q.Value()<<" "<<QR::Sign<<U::Sign();
}

//~ template<typename U,typename SIPrefix<86400,1> QR,typename T1 = double>
//~ std::ostream& operator<<(std::ostream& out, const Quantity<U,QR,T1>& q)
//~ {
	//~ return out<<q.Value()<<" "<<QR::Sign;
//~ }


#endif
