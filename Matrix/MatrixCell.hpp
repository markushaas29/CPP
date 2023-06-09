#include <memory>
#include "../Quantity/Quantity.hpp"

#pragma once

class MatrixCell
{
public:
	virtual std::unique_ptr<MatrixCell> Clone() = 0;
private:
};

template<typename V>
class ValueCell: public MatrixCell
{
public:
	ValueCell(V v): value{v} {};
	std::unique_ptr<MatrixCell> Clone() { return std::make_unique<ValueCell<V>>(*this); }
	V value;
private:
	friend std::ostream& operator<<(std::ostream& s, const ValueCell& vc) { return s<<vc.value;  }
};

template<typename UT, typename QR,typename T1>
class QuantityCell: public MatrixCell
{
	using Type = Quantity<UT,QR,T1>;
	QuantityCell(double v): quantity{v} {};
public:
	std::unique_ptr<MatrixCell> Clone() { return std::make_unique<QuantityCell<UT,QR,T1>>(*this); }
private:
	Type quantity;
};
