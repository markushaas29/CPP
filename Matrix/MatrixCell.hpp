#include <memory>
#include "../Is/Is.hpp"
#include "../String/Literal.hpp"
#include "../Quantity/Quantity.hpp"
#include "../Quantity/QuantityRatio.hpp"

#pragma once

class MatrixCell
{
public:
	virtual std::unique_ptr<MatrixCell> Clone() = 0;
	virtual std::ostream& Display(std::ostream& s) = 0;
private:
};

template<typename V>
class ValueCell: public MatrixCell
{
	inline static constexpr const char TypeIdentifier[] = "ValueCell";
	inline static constexpr Literal LiteralType{TypeIdentifier};
public:
	ValueCell(V v): value{v} {};
	std::unique_ptr<MatrixCell> Clone() { return std::make_unique<ValueCell<V>>(*this); }
	std::ostream& Display(std::ostream& os) { return os<<LiteralType; }
	V value;
private:
	friend std::ostream& operator<<(std::ostream& s, const ValueCell& vc) { return s<<vc.value;  }
};

//template<typename UT, typename QR=Pure,typename T1=double>
template<typename QT>
class QuantityCell: public MatrixCell
{
	using Type = QT;
	inline static constexpr const char TypeIdentifier[] = "QCell";
	inline static constexpr Literal LiteralType{TypeIdentifier};
public:
	QuantityCell(double v): quantity{v} {};
	std::unique_ptr<MatrixCell> Clone() { return std::make_unique<QuantityCell<Type>>(*this); }
	std::ostream& Display(std::ostream& os) { return os<<LiteralType; }
private:
	Type quantity;
};
