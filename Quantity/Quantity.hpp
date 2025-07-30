#include <string>
#include <iostream>
#include <cmath>
#include <sstream>
#include <regex>
#include "IQuantity.hpp"
#include "QuantityRatio.hpp"
#include "../Unit/Unit.hpp"
#include "../Unit/UnitRatio.hpp"
#include "../Logger/Logger.hpp"
#include "../String/StringParser.hpp"
#include "../Common/Make/Make.hpp"
#include "../String/String_.hpp"
#include "../To/To.hpp"

#ifndef QUANTITY_H
#define QUANTITY_H

template<typename> class Element; 

template<typename U, typename QR = Pure,typename T1 = double>
class Quantity: public Element<Quantity<U,QR,T1>>, public IQuantity<T1>
{
public:
	using Type = Quantity<U,QR,T1>;	
	using ValueType = T1;
	using UnitType = U;
	using QuantityRatioType = QR;
	using PureType = Quantity<U,Pure,T1>;	
	using Base = Element<Quantity<U,QR,T1>>;

    const std::string UnitSign() const { return U::Sign(); }
	const std::string SiUnit() const { return UnitType::SiUnit(); }
    inline static const std::string Identifier = U::Name;
    
	Quantity(): Base(data(0)), value(0 * QR::Factor) {	}
	explicit constexpr Quantity(const T1& v): Base(data(v)), value(v * QR::Factor) {	}
	explicit Quantity(const std::string& s): Base(data(stringTo<ValueType>(s))), value{(stringTo<ValueType>(s)) * (ValueType)QR::Factor} { 	}
	
	constexpr T1 Value() const { return value / QR::Factor;}
	constexpr T1 PureValue() const { return value;}
	constexpr decltype(auto) ToPure() const { return PureType(value);}
	
	template<typename U2 = U, typename SiPrefix2 = QR, typename T2 = T1>
	Quantity(Quantity<U2,SiPrefix2,T2> q ): Base(q.Data()),value(q.Value()){ Logger::Log()<<"CopyValue: "<<value<<std::endl;	}
	
	template<typename U2 = U, typename SiPrefix2 = QR, typename T2 = T1>
	auto Transform() { return transform(*this);	}
	
	template<typename SiPrefix = QR>
	auto In(){ return Quantity<U,SiPrefix,T1>(value / SiPrefix::Factor);	}
	
	constexpr bool Equals(const Quantity<U,QR,T1>& y, double epsilon = 0.001) const 	{	return std::fabs(Value()-y.Value()) <= epsilon; }
	constexpr decltype(auto) operator<=>(const Quantity<U,QR,T1>& y) const { return value <=> y.value; }
	constexpr bool operator==(const Quantity<U,QR,T1>& y) const {		return value == y.value; }
	constexpr operator ValueType() const { return Value(); }

	static decltype(auto) Create(std::istream& is)
	{
		ValueType val;
		is>>val;
		return Type{val};
	}

	constexpr decltype(auto) Pow(const int i) const { return *this * (*this);}
	
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
	friend class Element<Quantity<U,QR,T1>>;
	inline static std::string check(const std::string& iban) { return iban ; }
	T1 value;
	friend std::ostream& operator<<(std::ostream& out, const Quantity& q)	{	return out<<q.Data();	}
	friend std::istream& operator>>(std::istream& s, Quantity& q) 
	{
		std::string str;
		s>>str;
		auto v = stringTo<ValueType>(str);
		q = Quantity{v};
		return s; 
	}
	
	static decltype(auto) data(ValueType v) 
	{ 
		if constexpr (std::is_same_v<U, Sum>)
		{
			std::ostringstream oss;
			oss<<std::setprecision(2)<<std::fixed<<v;
			return oss.str()+QR::template TransformUnit<U>();
		}
		
		std::string res;
		if constexpr (std::is_same_v<T1, double>)
			res = String_::TrimDouble(v)+QR::template TransformUnit<U>(); 
		else
			res = std::to_string(v)+QR::template TransformUnit<U>(); 
		std::string str(res);
		std::size_t id = str.find_first_not_of("-+0123456789");

		if (id>=str.size())
			return res;

		auto result = str.substr(0,id);
		str = str.substr(id,str.size());
		
		id = str.find_first_not_of("0.,");
		if(id < str.size())
		{
			id = id > str.size() ? 0 : id;
			auto ending = str.substr(id,str.size());
			result += isdigit(str[0]) || (str[0]=='.'||str[0]==',') && (isdigit(str[1])) ? str :ending;
		}

		str.erase(remove_if(str.begin(), str.end(), [&](auto c){ return !isdigit(c) && c != '.'  && c != ',' && c != '-'; }), str.end());

		return result;
	}

	template<typename V>
	static decltype(auto) stringTo(const std::string& s)
	{
		std::string str(s);
		std::size_t id = str.find_first_not_of("-+0123456789.,");
		str = str.substr(0,id);
		str.erase(remove_if(str.begin(), str.end(), [&](auto c){ return !isdigit(c) && c != '.'  && c != ',' && c != '-'; }), str.end());
		return To<ValueType>(str);
	}
	
	template<typename TQuantity>
	constexpr static decltype(auto) transform(TQuantity t)
	{ 
		using TU = typename TQuantity::UnitType;
		using TQR = typename QR::RatioType<TQuantity::QuantityRatioType::N>;
		return Quantity<TU,TQR>(t.PureValue() / TQR::Factor);
	}
	
	template<typename U2 = U, typename TQR = QR, typename T2>
	constexpr decltype(auto) multiply(const Quantity<U2, TQR,T2>& q) const	{ 	return Quantity<typename Transform<U, U2, MultiplyPolicy>::Type, typename QR::Divider<TQR>::Result,T1>(Value() * q.Value());	}
	
	template<typename U2 = U, typename TQR = QR, typename T2>
	constexpr decltype(auto) divide(const Quantity<U2, TQR,T2>& q) const
	{ 
		constexpr int ex = QR::N - TQR::N;
		using QR_ = typename QR::Divider2<TQR>::Result;

		
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

using QA = Quantity<Area>;
using QC = Quantity<Current>;
using QE = Quantity<Energy>;
using QL = Quantity<Length>;
using QM = Quantity<Mass>;
using QS = Quantity<Sum>;
using QSC = Quantity<Scalar>;
using QT = Quantity<Time>;
using QTP = Quantity<Temperature>;
using QV = Quantity<Volume>;

#endif
