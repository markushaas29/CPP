#include <memory>
#include "../Is/Is.hpp"
#include "../CSV/Element.hpp"
#include "../String/Literal.hpp"
#include "../String/To/To.hpp"
#include "../Quantity/Quantity.hpp"
#include "../Quantity/QuantityRatio.hpp"

#pragma once

template<typename T=int>
class MatrixElement: public IMatrixElement
{
public:
	using PtrType = std::unique_ptr<Element<T>>;
	using Derived = D;
	decltype(auto) Get() const { return T{element->Value()}; }
	decltype(auto) Cast() const { return static_cast<const Derived*>(this); }
	PtrType Ptr() const { return std::make_unique<T>((element->Value()).c_str()); }
protected:
	MatrixElement(PtrType p): element{std::move(p)} {  }
	MatrixElement(const MatrixElement& m): element{std::make_unique<T>((element->Value()).c_str())} {  }
private:
	friend std::ostream& operator<<(std::ostream& s, const MatrixElement& me) { return s<<*me;  }
	PtrType element;
};
