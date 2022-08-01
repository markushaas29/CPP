#include <memory>
#include <ratio>
#include "../Wrapper/Wrapper.hpp"

#ifndef CALCULATORRESULT_HPP
#define CALCULATORRESULT_HPP


	struct QuantityRatio; //Fwd
	
	template<class Derived, typename L, typename R=L, typename V=L>
	class Result
	{
	public:
		using Type = Derived;
		using ValueType = V;
		using LeftType = L;
		using RightType = R;
		
		Result(LeftType l, RightType r, ValueType v): left{l}, right{r}, value{v}{}
		decltype(auto) Get() { return this->value; }
		std::ostream& Display(std::ostream& strm) const	
		{
			if constexpr (std::is_same<Type,QuantityRatio>::value)
			{
				return strm<<Derived::Name<<"\tTEST"<<left<<" "<<Derived::Sign<<" "<<right<<" = "<<value;
			}	
			return strm<<Derived::Name<<"\t"<<left<<" "<<Derived::Sign<<" "<<right<<" = "<<value;	
		}
	private:
		LeftType left;
		RightType right;
		ValueType value;
	};
	
	template<typename D, typename L, typename R=L, typename V=L>
	std::ostream& operator<<(std::ostream& strm, const Result<D,L,R,V> cr)
	{
		return cr.Display(strm);
	}
	
	template<typename TReading, typename TQuantity>
	struct ReadingResult			
	{
		using ReadingType = TReading;
		using QuantityType = TQuantity;
		const TReading FirstReading;
		const TReading SecondReading;
		const TQuantity Value;
		
		template<typename Separator = T::char_<'\t'>>
		std::ostream& Display(std::ostream& out) const
		{
			return out<<FirstReading<<Separator::Value<<SecondReading<<Separator::Value<<Value<<std::endl;
		}
		
		ReadingResult(TReading r1, TReading r2, TQuantity q): FirstReading(r1), SecondReading(r2), Value(q) {};	
	};

	template<typename R, typename Q, typename S = T::char_<'\t'>>
	std::ostream& operator<<(std::ostream& strm, const ReadingResult<R,Q> cr)
	{
		return cr.Display(strm);
	}



#endif
