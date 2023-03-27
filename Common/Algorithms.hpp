#include <memory>
#include <ratio>
#include <chrono>

#ifndef ALGOTITHMS_HPP
#define ALGOTITHMS_HPP

namespace Algorithms
{
	template<typename T>
	struct FunctorBase
	{
		using Type = std::shared_ptr<T>;
		Type Value() const { return this->value; }
		FunctorBase(Type t):value{t} {}
		virtual bool operator() (Type i) = 0;
	protected:
		Type value;
	};
	
	template<typename T>
	struct Equal: public FunctorBase<T>
	{
		using Base = FunctorBase<T>;
		Equal(Base::Type t):Base(t) {}
		bool operator() (Base::Type i){ return this->value == i;};
	};
	
	template<typename T>
	struct Equal<std::shared_ptr<T>>: public FunctorBase<T>
	{
		using Base = FunctorBase<T>;
		Equal(Base::Type t):Base(t) {}
		bool operator() (Base::Type i){ return this->value == i;};
	};
	
	template<typename T>
	struct NotEqual: public Equal<T>
	{
		using Base = Equal<T>;
		NotEqual(Base::Type t):Base(t) {}
		bool operator() (Base::Type i){ return !Base::operator()(i);};
	};
	
	template<typename InputIt, typename OutputIt, typename Predicate>
	OutputIt copy_if(InputIt begin, InputIt end, OutputIt dest, Predicate predicate)
	{
		while(begin != end)
		{
			if(predicate(*begin))
				*dest = *begin;

			++dest; 
			++begin;
		}
		
		return dest;
	}
}


#endif
