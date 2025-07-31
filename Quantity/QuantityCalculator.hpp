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

#pragma once
template<typename> class Element; 

template<typename U, typename QR = Pure,typename T1 = double>
class QuantityCalculator
{
public:
	using Type = Quantity<U,QR,T1>;	
	using ValueType = T1;
	using UnitType = U;
	using QuantityRatioType = QR;
	using PureType = Quantity<U,Pure,T1>;	
	using Base = Element<Quantity<U,QR,T1>>;
private:
	friend class Element<Quantity<U,QR,T1>>;
	friend class Quantity<U,QR,T1>;
	inline static std::string check(const std::string& iban) { return iban ; }
	
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
		return Quantity<TU,TQR,T1>(t.PureValue() / TQR::Factor);
	}
	
	template<typename U2 = U, typename TQR = QR, typename T2>
	constexpr static decltype(auto) multiply(const auto& q1,const Quantity<U2, TQR,T2>& q)	{ 	return Quantity<typename Transform<U, U2, MultiplyPolicy>::Type, typename QR::Divider<TQR>::Result,T1>(q1.VValue() * q.Value());	}
	
	template<typename U2 = U, typename TQR = QR, typename T2>
	constexpr static decltype(auto) divide(const auto& q1, const Quantity<U2, TQR,T2>& q) 
	{ 
		constexpr int ex = QR::N - TQR::N;
		using QR_ = typename QR::Divider2<TQR>::Result;

		
		if constexpr (TQR::BaseNum == QuantityRatioType::BaseNum && TQR::BaseDenom == QuantityRatioType::BaseDenom )
		{
			if constexpr (IsSameBaseUnit<U,U2>())
				return Quantity<typename Transform<U, U2, DividePolicy>::Type, QR_,T1>(q1.Value() / q.Value());
			
			return Quantity<typename Transform<U, U2, DividePolicy>::Type, QR_,T1>(q1.Value() / q.Value());
		}
		
		if constexpr (IsSameBaseUnit<U,U2>())
			return Quantity<typename Transform<U, U2, DividePolicy>::Type, QR_,T1>(q1.Value() / transform(q).Value());
		
		return Quantity<typename Transform<U, U2, DividePolicy>::Type, QR_,T1>(q1.Value() / q.PureValue());
	}
};
