#include <memory>
#include <ratio>
#include "CalculatorResult.hpp"
#include "../Fraction/Fraction.h"
#include "../Wrapper/Wrapper.hpp"
#include "../Logger/Logger.hpp"
#include "../File/Raiffeisenbank.hpp"
#include "../Common/DateTimes.hpp"

#ifndef OPERATIONS_HPP
#define OPERATIONS_HPP

	//-------------------------------------------------------------------------------------------------CalculatorOperation----------------------------------------------------------
	template<class Derived>
	struct CalculatorOperation
	{
		using Type = Derived;
	};
	
	struct Addition: CalculatorOperation<Addition>
	{ 
		inline static constexpr const char* Name = "Addition";
		inline static constexpr const char* Sign = "+";
		
		template<typename T, typename U=T>
		static constexpr decltype(auto) Calculate(const T& t1, const U& t2) { return Result<Addition,T,U,decltype(t1+t2)>(t1,t2,t1 + t2); }
	};
	
	struct Subtraction: CalculatorOperation<Subtraction>
	{ 
		inline static constexpr const char* Name = "Subtraction";
		inline static constexpr const char* Sign = "-";
		
		template<typename T, typename U=T>
		static constexpr decltype(auto) Calculate(const T& t1, const U& t2) { return Result<Subtraction,T,U,decltype(t1-t2)>(t1,t2,t1 - t2); }
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
	
	struct QuantityFraction: CalculatorOperation<QuantityFraction>
	{ 
		inline static constexpr const char* CalculatorOperation<QuantityFraction>::Name = "QuantityFraction";
		inline static constexpr const char* CalculatorOperation<QuantityFraction>::Sign = "%";
		
		template<typename L, typename R=L, typename Q=L>
		static constexpr decltype(auto) Calculate(const L& nom, const R& denom, const Q& q) {	
			auto fraction = nom / denom;
			using FractionType =  decltype(fraction);
			return Result<QuantityFraction,FractionType,Q,decltype((nom/denom) * q)>(fraction,q,fraction*q);	}
	};



#endif
