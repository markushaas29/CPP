#include <memory>
#include <ratio>
//~ #include "CalculatorResult.hpp"
#include "../Fraction/Fraction.h"
#include "../Wrapper/Wrapper.hpp"
#include "../Logger/Logger.hpp"
#include "../File/Raiba.hpp"
#include "../File/Account.hpp"
#include "../Common/DateTimes.hpp"

#ifndef OPERATIONS_HPP
#define OPERATIONS_HPP

	//-------------------------------------------------------------------------------------------------CalculatorOperation----------------------------------------------------------
	template<class Derived>
	struct CalculatorOperation
	{
		using Type = Derived;
	};
	
	template<class Derived, typename L, typename R=L, typename V=L>
	class Result
	{
	public:
		using Type = Derived;
		using ValueType = V;
		using LeftType = L;
		using RightType = R;
		Result(LeftType l, RightType r, ValueType v): left{l}, right{r}, value{v}{}
		std::ostream& Display(std::ostream& strm) const
		{
			return strm<<Derived::Name<<"\t"<<left<<" "<<Derived::Sign<<" "<<right<<" = "<<value;
		}
	//~ private;
		LeftType left;
		RightType right;
		ValueType value;
	};
	
	template<typename D, typename L, typename R=L, typename V=L>
	std::ostream& operator<<(std::ostream& strm, const Result<D,L,R,V> cr)
	{
		return cr.Display(strm);
	}
	
	struct Addition: CalculatorOperation<Addition>
	{ 
		inline static constexpr const char* Name = "Addition";
		inline static constexpr const char* Sign = "+";
		
		template<typename T>
		static constexpr decltype(auto) Calculate(const T& t1, const T& t2) {	return Result<Addition,T>(t1,t2,t1 + t2); }
	};
	
	struct Subtraction: CalculatorOperation<Subtraction>
	{ 
		inline static constexpr const char* Name = "Subtraction";
		inline static constexpr const char* Sign = "-";
		
		template<typename T>
		static constexpr decltype(auto) Calculate(const T& t1, const T& t2) { return Result<Subtraction,T>(t1,t2,t1 - t2); }
	};
	
	struct Multiplication: CalculatorOperation<Multiplication>
	{ 
		inline static constexpr const char* Name = "Multiplication";
		inline static constexpr const char* Sign = "*";
		
		template<typename T, typename U=T>
		static constexpr decltype(auto) Calculate(const T& t1, const U& t2) { return Result<Multiplication,T,U,decltype(t1*t2)>(t1,t2,t1 * t2); }
	};
	
	struct Division: CalculatorOperation<Division>
	{ 
		inline static constexpr const char* Name = "Division";
		inline static constexpr const char* Sign = "/";
		
		template<typename T, typename U=T>
		static constexpr decltype(auto) Calculate(const T& t1, const U& t2) { return Result<Division,T,U,decltype(t1 / t2)>(t1,t2,t1 / t2); }
	};

	struct Ratio: CalculatorOperation<Ratio>
	{ 
		inline static constexpr const char* CalculatorOperation<Ratio>::Name = "Ratio";
		inline static constexpr const char* CalculatorOperation<Ratio>::Sign = "%";
		
		template<typename T, typename Q = T>
		static constexpr decltype(auto) Calculate(const T& nom, const T& denom, const Q& sum) {	return Multiplication((Division::Calculate(nom,denom)),sum); }		
	};
	
	struct QuantityRatio: CalculatorOperation<QuantityRatio>
	{ 
		inline static constexpr const char* CalculatorOperation<QuantityRatio>::Name = "QuantityRatio";
		inline static constexpr const char* CalculatorOperation<QuantityRatio>::Sign = "%";
		
		template<typename T, typename Q = T>
		static constexpr decltype(auto) Calculate(const T& nom, const T& denom, const Q& q) {	return Q{Division::Calculate(nom,denom).Value() * q.Value()}; }
	};



#endif
