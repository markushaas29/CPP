#include <memory>
#include <tuple>
#include <vector>
#include "../Is/Is.hpp"
#include "../String/Literal.hpp"
#include "../CSV/Elements.hpp"    

#pragma once

template<std::size_t, typename> class Matrix;

template<typename Q, typename M>
class ICompositeResult
{
public:
	using QuantityType = Q;
	using MatrixType = M;
	virtual Q Value() const = 0;
private:
	friend 	std::ostream& operator<<(std::ostream& out, const ICompositeResult& s) {	return s.display(out);	}
	virtual std::ostream& display(std::ostream& out)	const = 0;
};

template<typename Q, typename M>
class MatrixCompositionResult: public ICompositeResult<Q,M>
{
	using Base = ICompositeResult<Q,M>;
public:
	MatrixCompositionResult(const Q&& q, const M&& m = M(), const std::string& n =""): value{q}, item(m), name{n} {};
	virtual Q Value() const { return value; }
private:
	friend 	std::ostream& operator<<(std::ostream& out, const MatrixCompositionResult& s)	{	return out<<"\nName: "<<s.name<<"\n"<<s.item<<"\n\nValue: "<<s.value;	}
	std::ostream& display(std::ostream& out) const { return out<<(*this); }
	typename Base::QuantityType value;
	M item;
	std::string name;
};

template<typename Q, typename M>
class MatrixCompositeResult: public ICompositeResult<Q,M>
{
	using Base = ICompositeResult<Q,M>;
public:
	MatrixCompositeResult(const Q&& q, const std::vector<std::unique_ptr<Base>>&& v, const std::string& n =""): value{q}, name{n} {};
	virtual Q Value() const { return value; }
private:
	friend 	std::ostream& operator<<(std::ostream& out, const MatrixCompositeResult& s)	{	return out<<"\nName: "<<s.name<<"\n"<<s.item<<"\n\nValue: "<<s.value;	}
	std::ostream& display(std::ostream& out) const { return out<<(*this); }
	typename Base::QuantityType value;
	M item;
	std::string name;
};
