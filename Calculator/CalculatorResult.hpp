#include <memory>
#include <ratio>
#include "../Wrapper/Wrapper.hpp"

#ifndef CALCULATORRESULT_HPP
#define CALCULATORRESULT_HPP

	struct Addition; //Fwd
	struct Subtraction; //Fwd
	struct Multiplication; //Fwd
	struct Division; //Fwd

	template<class Derived, typename L, typename R, typename V> class Result;

	template<typename T> concept IsResultType = std::is_same<T,Result<typename T::Type, typename T::LeftType,typename T::RightType,typename T::ValueType>>::value;
	
	template<typename T>
	concept IsResult = requires(T t) 
	{
		t.Get();
		T::LeftType;
		T::RightType;
		T::ValueType;
		T::ResultType;
	 } && IsResultType<T>;
	
	template<class Derived, typename L, typename R=L, typename V=L>
	class Result
	{
	public:
		using Type = Derived;
		using ValueType = V;
		using LeftType = L;
		using RightType = R;
		using ResultType = Result<Type,LeftType,RightType,ValueType>;
		
		constexpr Result(LeftType l, RightType r, ValueType v): left{l}, right{r}, value{v}{}
		constexpr Result() = default;
		constexpr decltype(auto) Get() const 
		{
			if constexpr (IsResult<decltype(value)>)
				return value.Get();
			return value; 
		}

		template<typename T>
		constexpr operator T() const { return static_cast<T>(value); }
		std::ostream& Display(std::ostream& strm) const	{ return strm<<"{"<<left<<"} "<<Derived::Sign<<" {"<<right<<"} = "<<value; }
	private:
		LeftType left;
		RightType right;
		ValueType value;
	};
	
	struct Accumulation;
	struct DotProduct;

	template<typename L, typename R, typename V>
	class Result<DotProduct,L,R,V>
	{
	public:
		using Type = DotProduct;
		using ValueType = V;
		using LeftType = L;
		using RightType = R;
		using ResultType = Result<Type,LeftType,LeftType,ValueType>;
		
		constexpr Result(LeftType l, ValueType v): left{l}, value{v}{}
		constexpr Result() = default;
		constexpr decltype(auto) Get() const 
		{
			if constexpr (IsResult<decltype(value)>)
				return value.Get();
			return value; 
		}

		template<typename T>
		constexpr operator T() const { return static_cast<T>(value); }
		std::ostream& Display(std::ostream& strm) const	
		{
			for(uint i = 0; i < left.size(); ++i)
				strm<<"{"<<*left[i]<<"}"<<( (i+1) != left.size() ? "+" : "");
			strm<<" = "<<value;
			return strm; 
		}
	private:
		LeftType left;
		ValueType value;
	};

	template<typename L, typename R, typename V>
	class Result<Accumulation,L,R,V>
	{
	public:
		using Type = Accumulation;
		using ValueType = V;
		using LeftType = L;
		using RightType = R;
		using ResultType = Result<Type,LeftType,LeftType,ValueType>;
		
		constexpr Result(LeftType l, ValueType v): left{l}, value{v}{}
		constexpr Result() = default;
		constexpr decltype(auto) Get() const 
		{
			if constexpr (IsResult<decltype(value)>)
				return value.Get();
			return value; 
		}

		template<typename T>
		constexpr operator T() const { return static_cast<T>(value); }
		std::ostream& Display(std::ostream& strm) const	
		{
			for(uint i = 0; i < left.size(); ++i)
				strm<<"{"<<*left[i]<<"}"<<( (i+1) != left.size() ? "+" : "");//<<" = "<<value;
			strm<<" = "<<value;
			return strm; 
		}
	private:
		LeftType left;
		ValueType value;
	};
	template<typename D, typename L, typename R=L, typename V=L>
	std::ostream& operator<<(std::ostream& strm, const Result<D,L,R,V> cr)	{	return cr.Display(strm);}
	
	//------------------------------------------Addition-------------------------------------------------------------

	template<class D1, typename L1, typename R1=L1, typename V1=L1,class D2, typename L2, typename R2=L2, typename V2=L2>
	constexpr decltype(auto) operator+(const Result<D1,L1,R1,V1>& r1, const Result<D2,L2,R2,V2>& r2) { return Result<Addition,Result<D1,L1,R1,V1>,Result<D2,L2,R2,V2>, decltype(r1.Get() +  r2.Get())>(r1,r2,r1.Get() + r2.Get()); }
	
	template<typename T, class D, typename L, typename R=L, typename RT=T>
	constexpr decltype(auto) operator+(const Result<D,L,R,RT>& r, const T& t) { return Result<Addition,Result<D,L,R,RT>, T, decltype(r.Get() + t)>(r,t,r.Get() + t); }
	
	template<typename T, class D, typename L, typename R=L, typename RT=T>
	constexpr decltype(auto) operator+(const T& t, const Result<D,L,R,RT>& r) { return Result<Addition,T,Result<D,L,R,RT>, decltype(t + r.Get())>(t,r,t + r.Get()); }
	
	//------------------------------------------Subtraction-------------------------------------------------------------

	template<class D1, typename L1, typename R1=L1, typename V1=L1,class D2, typename L2, typename R2=L2, typename V2=L2>
	constexpr decltype(auto) operator-(const Result<D1,L1,R1,V1>& r1, const Result<D2,L2,R2,V2>& r2) { return Result<Subtraction,Result<D1,L1,R1,V1>,Result<D2,L2,R2,V2>, decltype(r1.Get() -  r2.Get())>(r1,r2,r1.Get() - r2.Get()); }
	
	template<typename T, class D, typename L, typename R=L, typename RT=T>
	constexpr decltype(auto) operator-(const Result<D,L,R,RT>& r, const T& t) { return Result<Subtraction,Result<D,L,R,RT>, T, decltype(r.Get() - t)>(r,t,r.Get() - t); }
	
	template<typename T, class D, typename L, typename R=L, typename RT=T>
	constexpr decltype(auto) operator-(const T& t, const Result<D,L,R,RT>& r) { return Result<Subtraction,T,Result<D,L,R,RT>, decltype(t - r.Get())>(t,r,t - r.Get()); }
	
	//------------------------------------------Multiplication-------------------------------------------------------------

	template<typename T, class D, typename L, typename R=L, typename V=L>
	constexpr decltype(auto) operator*(const Result<D,L,R,V>& r, const T& t) { return Result<Multiplication,Result<D,L,R,V>, T, decltype(r.Get() * t)>(r,t,r.Get() * t); }
	
	template<typename T, class D, typename L, typename R=L, typename V=L>
	constexpr decltype(auto) operator*(const T& t, const Result<D,L,R,V>& r) { return Result<Multiplication,T,Result<D,L,R,V>, decltype(t *  r.Get())>(t,r,t * r.Get()); }
	
	template<class D1, typename L1, typename R1=L1, typename V1=L1,class D2, typename L2, typename R2=L2, typename V2=L2>
	constexpr decltype(auto) operator*(const Result<D1,L1,R1,V1>& r1, const Result<D2,L2,R2,V2>& r2) { return Result<Multiplication,Result<D1,L1,R1,V1>,Result<D2,L2,R2,V2>, decltype(r1.Get() *  r2.Get())>(r1,r2,r1.Get() * r2.Get()); }
	
	//------------------------------------------Division-------------------------------------------------------------
	
	template<typename T, class D, typename L, typename R=L, typename V=L>
	constexpr decltype(auto) operator/(const Result<D,L,R,V>& r, const T& t) { return Result<Division,Result<D,L,R,V>, T, decltype(r.Get() / t)>(r,t,r.Get() / t); }
	
	template<typename T, class D, typename L, typename R=L, typename V=L>
	constexpr decltype(auto) operator/(const T& t, const Result<D,L,R,V>& r) { return Result<Division,T,Result<D,L,R,V>, decltype(t /  r.Get())>(t,r,t / r.Get()); }
	
	template<class D1, typename L1, typename R1=L1, typename V1=L1,class D2, typename L2, typename R2=L2, typename V2=L2>
	constexpr decltype(auto) operator/(const Result<D1,L1,R1,V1>& r1, const Result<D2,L2,R2,V2>& r2) { return Result<Division,Result<D1,L1,R1,V1>,Result<D2,L2,R2,V2>, decltype(r1.Get() /  r2.Get())>(r1,r2,r1.Get() / r2.Get()); }

	template<typename U, typename Pre, typename Q, typename DateT> struct Reading; //Fwd
	
#endif
