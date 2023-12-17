#include <type_traits>
#include "ExpressionBase.hpp"
#include "UnaryExpression.hpp"
#include "BinaryExpression.hpp"
#include "BoolExpression.hpp"
//#include "VecBinary.hpp"
//#include "VecUnary.hpp"
//#include "ExpressionOperators.hpp"

#pragma once
//
//template<template<typename,typename> class Op, typename L, typename R>
//decltype(auto) Func(const L& l, const R& r) { return Op<L,R>(l,r); }
//
//template<template<typename> class C, typename T = double>
//decltype(auto) Func(const T& t = T{}) { return C<T>(t); }
