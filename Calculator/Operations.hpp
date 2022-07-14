#include <memory>
#include <ratio>
#include "CalculatorConfiguration.hpp"
#include "CalculatorResult.hpp"
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
	
	struct Addition: CalculatorOperation<Addition>
	{ 
		inline static constexpr const char* Name = "Addition";
		inline static constexpr const char* Sign = "+";
		
		template<typename T>
		static constexpr decltype(auto) Calculate(const T& t1, const T& t2) {	return t1 + t2; }
	};
	
	struct Subtraction: CalculatorOperation<Subtraction>
	{ 
		inline static constexpr const char* Name = "Subtraction";
		inline static constexpr const char* Sign = "-";
		
		template<typename T>
		static constexpr decltype(auto) Calculate(const T& t1, const T& t2) { return t1 - t2; }
	};
	
	struct Multiplication: CalculatorOperation<Multiplication>
	{ 
		inline static constexpr const char* Name = "Multiplication";
		inline static constexpr const char* Sign = "*";
		
		template<typename T>
		static constexpr decltype(auto) Calculate(const T& t1, const T& t2) { return t1 * t2; }
	};
	
	struct Division: CalculatorOperation<Division>
	{ 
		inline static constexpr const char* Name = "Division";
		inline static constexpr const char* Sign = "/";
		
		template<typename T>
		static constexpr decltype(auto) Calculate(const T& t1, const T& t2) { return t1 / t2; }
	};

	struct Ratio: CalculatorOperation<Ratio>
	{ 
		inline static constexpr const char* CalculatorOperation<Ratio>::Name = "Ratio";
		inline static constexpr const char* CalculatorOperation<Ratio>::Sign = "%";
		
		template<typename T, typename Q = T>
		static constexpr decltype(auto) Calculate(const T& nom, const T& denom, const Q& sum) {	return Multiplication((Division::Calculate(nom,denom)),sum); }
		
		//~ template<typename T, typename U, typename S, typename D>
		//~ static constexpr decltype(auto) Calculate(const T& nom, const T& denom, const Quantity<T,U,D>& sum) {	return Multiplication((Division::Calculate(nom,denom)),sum.Value()); }
	};
	
	struct QuantityRatio: CalculatorOperation<QuantityRatio>
	{ 
		inline static constexpr const char* CalculatorOperation<QuantityRatio>::Name = "QuantityRatio";
		inline static constexpr const char* CalculatorOperation<QuantityRatio>::Sign = "%";
		
		template<typename T, typename Q = T>
		static constexpr decltype(auto) Calculate(const T& nom, const T& denom, const Q& q) {	return Q{Division::Calculate(nom,denom).Value() * q.Value()}; }
	};



#endif
