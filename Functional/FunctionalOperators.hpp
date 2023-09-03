#include "Unary.hpp"         
#include "Binary.hpp"

#pragma once

template<template<typename,typename> class D1,typename L1, typename R1, template<typename,typename> class D2,typename L2, typename R2>
constexpr decltype(auto) operator+(const BinaryFunctional<D1,L1,R1>& f1, const BinaryFunctional<D2,L2,R2>& f2) { return Func<Add>(f1,f2); }

template<template<typename,typename> class D1,typename L1, typename R1, template<typename> class D2,typename V2>
constexpr decltype(auto) operator+(const BinaryFunctional<D1,L1,R1>& f1, const UnaryFunctional<D2,V2>& f2) { return Func<Add>(f1,f2); }

template<template<typename,typename> class D1,typename L1, typename R1, template<typename> class D2,typename V2>
constexpr decltype(auto) operator+(const UnaryFunctional<D2,V2>& f1, const BinaryFunctional<D1,L1,R1>& f2) { return Func<Add>(f1,f2); }

template<template<typename,typename> class D1,typename L1, typename R1, template<typename,typename> class D2,typename L2, typename R2>
constexpr decltype(auto) operator-(const BinaryFunctional<D1,L1,R1>& f1, const BinaryFunctional<D2,L2,R2>& f2) { return Func<Sub>(f1,f2); }

template<template<typename,typename> class D1,typename L1, typename R1, template<typename> class D2,typename V2>
constexpr decltype(auto) operator-(const BinaryFunctional<D1,L1,R1>& f1, const UnaryFunctional<D2,V2>& f2) { return Func<Sub>(f1,f2); }

template<template<typename,typename> class D1,typename L1, typename R1, template<typename> class D2,typename V2>
constexpr decltype(auto) operator-(const UnaryFunctional<D2,V2>& f1, const BinaryFunctional<D1,L1,R1>& f2) { return Func<Sub>(f1,f2); }

template<template<typename,typename> class D1,typename L1, typename R1, template<typename,typename> class D2,typename L2, typename R2>
constexpr decltype(auto) operator*(const BinaryFunctional<D1,L1,R1>& f1, const BinaryFunctional<D2,L2,R2>& f2) { return Func<Mul>(f1,f2); }

template<template<typename,typename> class D1,typename L1, typename R1, template<typename> class D2,typename V2>
constexpr decltype(auto) operator*(const BinaryFunctional<D1,L1,R1>& f1, const UnaryFunctional<D2,V2>& f2) { return Func<Mul>(f1,f2); }

template<template<typename,typename> class D1,typename L1, typename R1, template<typename> class D2,typename V2>
constexpr decltype(auto) operator*(const UnaryFunctional<D2,V2>& f1, const BinaryFunctional<D1,L1,R1>& f2) { return Func<Mul>(f1,f2); }

template<template<typename,typename> class D1,typename L1, typename R1, template<typename,typename> class D2,typename L2, typename R2>
constexpr decltype(auto) operator/(const BinaryFunctional<D1,L1,R1>& f1, const BinaryFunctional<D2,L2,R2>& f2) { return Func<Div>(f1,f2); }

template<template<typename,typename> class D1,typename L1, typename R1, template<typename> class D2,typename V2>
constexpr decltype(auto) operator/(const BinaryFunctional<D1,L1,R1>& f1, const UnaryFunctional<D2,V2>& f2) { return Func<Div>(f1,f2); }

template<template<typename,typename> class D1,typename L1, typename R1, template<typename> class D2,typename V2>
constexpr decltype(auto) operator/(const UnaryFunctional<D2,V2>& f1, const BinaryFunctional<D1,L1,R1>& f2) { return Func<Div>(f1,f2); }
