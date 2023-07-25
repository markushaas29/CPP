#include <memory>
#include <ratio>
#include <vector>
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
	
	struct Accumulation: CalculatorOperation<DotProduct>
	{ 
		inline static constexpr const char* Name = "Sum";
		inline static constexpr const char* Sign = "+";
		
		template<typename T, typename U=T>
		static constexpr decltype(auto) Calculate(const std::vector<T>& v1) 
		{ 
			double r = 0;
			std::for_each(v1.cbegin(), v1.cend(), [&](const auto& i) {	r += (double)(i); });
			return Result<Accumulation,std::vector<T>,std::vector<T>,double>(v1,r); 
		}
		
		template<typename T, typename U=T>
		static constexpr decltype(auto) Calculate(const std::vector<std::shared_ptr<T>>& v1) 
		{ 
			double r = 0;
			std::for_each(v1.cbegin(), v1.cend(), [&](const auto& i) {	r += (double)(*i); });
			return Result<Accumulation,std::vector<std::shared_ptr<T>>,std::vector<std::shared_ptr<T>>,double>(v1,r); 
		}
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
	
	struct DotProduct: CalculatorOperation<DotProduct>
	{ 
		inline static constexpr const char* Name = "DotProduct";
		inline static constexpr const char* Sign = "+";
		
		template<typename T, typename U=T>
		static constexpr decltype(auto) Calculate(const std::vector<T>& v1, const std::vector<U>& v2) 
		{ 
			using RT = decltype(Multiplication::Calculate(v1[0],v2[0]));
			std::vector<RT> inter;
			for(uint i =0; i < v1.size(); ++i)
				inter.push_back(Multiplication::Calculate(v1[i],v2[i]));

			return Result<DotProduct,decltype(inter),std::vector<U>,double>(inter,(double)Accumulation::Calculate(inter)); 
		}
		
		template<typename T, typename U=T>
		static constexpr decltype(auto) Calculate(const std::vector<std::shared_ptr<T>>& v1, const std::vector<std::shared_ptr<U>>& v2) 
		{ 
			using RT = decltype(Multiplication::Calculate(*v1[0],*v2[0]));
			std::vector<std::shared_ptr<RT>> inter;
			for(uint i =0; i < v1.size(); ++i)
				inter.push_back(std::make_shared<RT>(Multiplication::Calculate(*v1[i],*v2[i])));

			return Result<DotProduct,decltype(inter),std::vector<U>,double>(inter,(double)Accumulation::Calculate(inter)); 
		}
	};

	struct Ratio: CalculatorOperation<Ratio>
	{ 
		inline static constexpr const char* CalculatorOperation<Ratio>::Name = "Ratio";
		inline static constexpr const char* CalculatorOperation<Ratio>::Sign = "%";
		
		template<typename L, typename R=L,typename Q = L>
		static constexpr decltype(auto) Calculate(const L& nom, const R& denom, const Q& sum) {	return Division::Calculate(nom,denom) * sum; }		
	};
	
#endif
