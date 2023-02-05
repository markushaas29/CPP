#include <string>
#include <iostream>

#ifndef TYPELIST_H
#define TYPELIST_H

template<typename... Elements>
class Typelist
{	
public:
	using Type = Typelist<Elements...>;
	static constexpr int Size = sizeof...(Elements);
	static constexpr bool Empty = Size == 0 ? true : false;
};

//--------------------------------------------------------------------------------
template<typename List>
class FrontT
{
};

template<typename Head, typename... Tail>
class FrontT<Typelist<Head, Tail...>>
{
public:
	using Type = Head;
};

template<typename List>
using Front = typename FrontT<List>::Type;

//--------------------------------------------------------------------------------
// template<typename List>
// class GeT
// {
// };
// 
// template<typename Head, typename... Tail>
// class GeT<Typelist<Head, Tail...>>
// {
// public:
// 	using Type = Head;
// };
// 
// template<typename List, typename T>
// using Get = typename GeT<List>::Type;

//--------------------------------------------------------------------------------
template<typename List>
class PopFrontT
{
};

template<typename Head, typename... Tail>
class PopFrontT<Typelist<Head, Tail...>>
{
public:
	using Type = Typelist<Tail...>;
};

template<typename List>
using PopFront = typename PopFrontT<List>::Type;

//--------------------------------------------------------------------------------
template<typename List, typename newHead>
class PushFrontT
{
};

template<typename... Elements, typename newHead>
class PushFrontT<Typelist<Elements...>, newHead>
{
public:
	using Type = Typelist<newHead, Elements...>;
};

template<typename List, typename newHead>
using PushFront = typename PushFrontT<List, newHead>::Type;

//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
template<typename List, typename NewElement>
class PushBackT
{
};

template<typename... Elements, typename NewElement>
class PushBackT<Typelist<Elements...>, NewElement>
{
public:
	using Type = Typelist<Elements..., NewElement>;
};

template<typename List, typename NewElement>
using PushBack = typename PushBackT<List, NewElement>::Type;

//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
template<typename List, unsigned N>
class AtT: public AtT<PopFront<List>, N-1>
{
};

template<typename List>
class AtT<List,0> : public FrontT<List>
{
};

template<typename List, unsigned N>
using At = typename AtT<List, N>::Type;

//--------------------------------------------------------------------------------


#endif
