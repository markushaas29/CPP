#include <string>
#include <iostream>
#include "../Traits/Traits.hpp"
#include "../Typelist/Typelist.hpp"

#ifndef WRAPPER_H
#define WRAPPER_H

namespace T
{
	
//------------------------------------------------------------------------------------BOOL---------------------------------------------------------------------------
template<bool B>
struct bool_
{
	static constexpr bool Value = B;
	using Type = bool_<B>;
	using ValueType = bool;
	operator bool() const { return  Value; }
};

using FalseType = bool_<false>;
using TrueType = bool_<true>;

//------------------------------------------------------------------------------------INT---------------------------------------------------------------------------
template<int N>
struct int_
{
	static constexpr int Value = N;
	using Type = int_<N>;
	using Next = int_<N+1>;
	using Previous = int_<N-1>;
	operator int() const { return  N; }
};

//------------------------------------------------------------------------------------CharTypeBase---------------------------------------------------------------------------
template<typename T,template<typename> class Derived> 
struct CharTypeBase
{
	static constexpr char Value = T::Value;
	using ValueType = typename T::Type;
	using Type = Derived<T>;
	using Previous = Derived<typename T::Previous>;
	using Next = Derived<typename T::Next>;
	operator char() const { return T::Value; };
	
};

struct None{};

template<typename T>
struct Num: public CharTypeBase<T, Num> { };

template<typename T>
struct Letter: public CharTypeBase<T, Letter> { };

template<typename T>
struct IsNum: public bool_<(T::Value > 47 && T::Value < 58)>{};

template<typename T>
struct IsLetter:public bool_<(T::Value > 64 && T::Value < 90)>{};

template<typename T>
struct IsNone:public bool_<(T::Value < 0)> {};


//------------------------------------------------------------------------------------Char_---------------------------------------------------------------------------
template<char N>
struct char_
{
	static constexpr bool IsNum = N > 47 && N < 58;
	static constexpr bool IsLetter = N > 64 && N < 90;
	static constexpr char Value = N;
	static constexpr int Num = N;
	inline static const std::string String = std::string(1,Value);
	using Type = char_<N>;
	using ValueType = char;
	using Next = char_<N+1>;
	
// 	auto Get(const int i) { return char_<Val<i>::Value>; };
	
	operator int() const { return  N; }
	operator char() const { return  N; }
	static bool Is(char c) 
	{ 
		std::cout<<"CHAR_IS Para:"<<c<<"== T:"<<Value<<std::endl;
		return c == Value; 
	}

	explicit char_() { }
};
//------------------------------------------------------------------------------------ValueType---------------------------------------------------------------------------

template<typename T, typename = void>
struct ValueType
{
	using Type = None;
};

template<typename T>
struct ValueType<T, Traits::EnableIf<IsNum<T>::Value>>
{
	using Type = typename Num<T>::Type;
};

template<typename T>
struct ValueType<T, Traits::EnableIf<IsLetter<T>::Value>>
{
	using Type = typename Letter<T>::Type;
};

template<int N>
struct Val
{
	enum {Value = N};
};
//------------------------------------------------------------------------------------TypeTraits---------------------------------------------------------------------------

template<typename T,template<T> class ValueType> struct Value { };

template<typename Type>
struct Is_ 
{ 
	bool operator()(typename Type::ValueType v) { return v == Type::Value; }
};

template<typename Type>
struct IsNot_ 
{ 
	bool operator()( typename Type::ValueType v) { return v != Type::Value; }
};



template<typename T, typename U>
struct IsSame: FalseType{ };

template<typename T>
struct IsSame<T,T>: TrueType{ };
}

//------------------------------------------------------------------------------------Transform---------------------------------------------------------------------------
namespace VL
{
	
	
template<typename T, template<T> class ValueType, T... values>
struct Transform;

template<typename T, template<T> class ValueType,T Head>
struct Transform<T,ValueType,Head>
{
	using Type = Typelist<ValueType<Head>>;
};

template<typename T, template<T> class ValueType, T  Head, T... Tail>
struct Transform<T,ValueType,Head,Tail...>
{
	using Type = PushFront<typename Transform<T, ValueType,Tail...>::Type, ValueType<Head>>;
};

//--------------------------------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------Valuelist-----------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------------------------------

template<typename T, T... Values>
struct Valuelist {
	using ValueType = T;
// 	using Type = Transform<T>::Type;
	static constexpr size_t Size = sizeof...(Values);
	
};

template<typename T, T... Values>
struct IsEmpty {	static constexpr bool value = sizeof...(Values) == 0; };

template<typename List>
struct Front {	};

template<typename T, T Head, T... Tail>
struct Front<Valuelist<T, Head, Tail...>> {
	using Type = T;
	static constexpr T Value = Head;
};

//~ template<typename List>
//~ struct PopFront {	};

//~ template<typename T, T Head, T... Tail>
//~ struct PopFront<Valuelist<T, Head, Tail...>> {
	//~ using Type = Valuelist<T, Tail...>;
	//~ static constexpr T Value = Head;
//~ };

//~ template<typename List, typename List::ValueType Head>
//~ struct PushFront {	};

//~ template<typename T, T Head, T... Tail>
//~ struct PushFront<Valuelist<T, Tail...>, Head> {
	//~ using Type = Valuelist<T, Head ,Tail...>;
//~ };

//~ template<typename List, typename List::ValueType End>
//~ struct PushBack {	};

//~ template<typename T, T End, T... Tail>
//~ struct PushBack<Valuelist<T, Tail...>, End> {
	//~ using Type = Valuelist<T, Tail..., End>;
//~ };


}
#endif
