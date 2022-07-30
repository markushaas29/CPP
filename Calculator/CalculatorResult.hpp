#include <memory>
#include <ratio>
#include "../Wrapper/Wrapper.hpp"

#ifndef CALCULATORRESULT_HPP
#define CALCULATORRESULT_HPP


	//~ template<typename Config>
	//~ struct Result			
	//~ {
		//~ using Configuration = Config;
		
		
		//~ template<typename Separator = T::char_<'\t'>>
		//~ std::ostream& Display(std::ostream& out) const
		//~ {
			//~ return out;
		//~ }
		
		//~ Result(){}
	//~ };
	
	//~ template<typename C, typename S = T::char_<'\t'>>
	//~ std::ostream& operator<<(std::ostream& strm, const Result<C> r)
	//~ {
		//~ return r.Display(strm);
	//~ }
	
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
