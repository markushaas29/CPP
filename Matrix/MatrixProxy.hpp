#include "../String/String_.hpp"
#include "../Logger/Logger.hpp"
#include "../Wrapper/Wrapper.hpp"

#ifndef MATRIXPROXY_H
#define MATRIXPROXY_H

//--------------------------------FORMAT_NON_ZERO------------------------------------------------

template<typename Arr>
class MatrixProxy
{
public:
	MatrixProxy(const Arr& a, size_t r): A{a}, r{r} {}
	auto operator[](size_t c) { return A(r,c); }
private:
	const Arr& A;
	size_t r;
};

template<typename T>
class OperatorProxy
{
public:
	using Type = MatrixProxy<const T>;
	Type operator[](size_t c) const { return {static_cast<const T&>(*this),c}; }
};

#endif
