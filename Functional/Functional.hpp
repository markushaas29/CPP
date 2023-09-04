#include <type_traits>
#include "FunctionalBase.hpp"
#include "Unary.hpp"
#include "Binary.hpp"
#include "FunctionalOperators.hpp"

#pragma once

template<template<typename,typename> class Op, typename L, typename R>
decltype(auto) Func(const L& l, const R& r) { return Op<L,R>(l,r); }

template<template<typename> class C, typename T = double>
decltype(auto) Func(const T& t = T{}) { return C<T>(t); }
