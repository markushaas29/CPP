#include "Parser.hpp"
#include "../String/String_.hpp"
#include "../String/To/To.hpp"
#include "../Logger/Logger.hpp"
#include "../Unit/Unit.hpp"
#include "../Quantity/Quantity.hpp"
#include "../Quantity/QuantityRatio.hpp"
#include "../Wrapper/Wrapper.hpp"
#include "../Common/DateTimes.hpp"
#include "../Calculator/CalculatorResult.hpp"

#ifndef READING_HPP
#define READING_HPP


template<typename U, typename Pre = Pure, typename Q = Quantity<U,Pure,double>, typename DateT = DateTimes::Date>
class Reading
{
public:
	using Unit = U;
	using QuantityType = Q;
	using DateType = DateT;
	using Type = Reading<Unit,Pre,QuantityType,DateType>;
	const DateType Date;
	const QuantityType QuantityValue;
	
	template<typename Separator = T::char_<';'>>
	std::ostream& Display(std::ostream& out) const	{ return out<<Date<<Separator::Value<<QuantityValue.Value()<<Separator::Value<<QuantityType::QuantityRatioType::Sign<<U::Sign();	}
	Reading(QuantityType val, DateType d): Date(d), QuantityValue(val)	{}
	Reading(): Date{1,1,2000}, QuantityValue{0}{};
	
	static decltype(auto) Create(const std::string& d, const std::string& v)
	{
		auto date = Parsers::Parser<std::string,DateType>::Parse(d);
		Logger::Log("Date: ",date); 
		auto value = String_::ParseTo<double,std::string>(v);
				
		return std::make_shared<Type>(QuantityType(value), DateType(date));
	}
	
	decltype(auto) operator+(const Type& left) const {return Result<Addition,Type,Type,QuantityType>(*this,left, this->QuantityValue + left.QuantityValue);}
	decltype(auto) operator-(const Type& left) const {return Result<Subtraction,Type,Type,QuantityType>(*this,left, this->QuantityValue - left.QuantityValue);}
	decltype(auto) operator*(const Type& left) const {return Result<Multiplication,Type,Type,decltype(this->QuantityValue * left.QuantityValue)>(*this,left, this->QuantityValue * left.QuantityValue);}
	decltype(auto) operator/(const Type& left) const {return Result<Division,Type,Type,decltype(this->QuantityValue / left.QuantityValue)>(*this,left, this->QuantityValue / left.QuantityValue);}
	
	decltype(auto) operator<=>(const Type& left) const {return this->QuantityValue <=> left.QuantityValue;}
	bool operator==(const Type& left) const {return this->QuantityValue == left.QuantityValue;}
};

template<typename U, typename Pre = Pure, typename Q = Quantity<U,Pre,double>, typename DateT = DateTimes::Date>
std::ostream& operator<<(std::ostream& strm, const Reading<U,Pre,Q,DateT>& r){	return r.Display(strm);}
//--------------------------------------------Addition---------------------------------------------------------

template<typename U,typename P, typename Q, typename D>
decltype(auto) operator+(const Q& q, const Reading<U,P,Q,D>& r) { return Result<Addition,Q,Reading<U,P,Q,D>,decltype(q + r.QuantityValue)>(q,r,q + r.QuantityValue);}

template<typename U,typename P, typename Q, typename D>
decltype(auto) operator+(const Reading<U,P,Q,D>& r, const Q& q) { return Result<Addition,Reading<U,P,Q,D>,Q,decltype(q + r.QuantityValue)>(r,q,q + r.QuantityValue);}

//--------------------------------------------Subtraction---------------------------------------------------------

template<typename U,typename P, typename Q, typename D>
decltype(auto) operator-(const Q& q, const Reading<U,P,Q,D>& r) { return Result<Subtraction,Q,Reading<U,P,Q,D>,decltype(q - r.QuantityValue)>(q,r,q - r.QuantityValue);}

template<typename U,typename P, typename Q, typename D>
decltype(auto) operator-(const Reading<U,P,Q,D>& r, const Q& q) { return Result<Subtraction,Reading<U,P,Q,D>,Q,decltype(r.QuantityValue - q)>(r,q,r.QuantityValue - q);}

//--------------------------------------------Multiplication---------------------------------------------------------

template<typename U,typename P, typename Q, typename D, typename QT>
decltype(auto) operator*(const QT& q, const Reading<U,P,Q,D>& r) { return Result<Multiplication,QT,Reading<U,P,Q,D>,decltype(q * r.QuantityValue)>(q,r,q * r.QuantityValue);}

template<typename U,typename P, typename Q, typename D, typename QT>
decltype(auto) operator*(const Reading<U,P,Q,D>& r, const QT& q) { return Result<Multiplication,Reading<U,P,Q,D>,QT,decltype(q * r.QuantityValue)>(r,q,q * r.QuantityValue);}

//--------------------------------------------Division---------------------------------------------------------

template<typename U,typename P, typename Q, typename D, typename QT>
decltype(auto) operator/(const QT& q, const Reading<U,P,Q,D>& r) { return Result<Division,QT,Reading<U,P,Q,D>,decltype(q / r.QuantityValue)>(q,r,q / r.QuantityValue);}

template<typename U,typename P, typename Q, typename D, typename QT>
decltype(auto) operator/(const Reading<U,P,Q,D>& r, const QT& q) { return Result<Division,Reading<U,P,Q,D>,QT,decltype(r.QuantityValue / q)>(r,q,r.QuantityValue / q);}

template<typename C,typename T = double, typename DateT = DateTimes::Date>
std::ostream& operator<<(std::ostream& strm, const Reading<C,T,DateT> c)	{	return c.Display(strm);}


#endif
