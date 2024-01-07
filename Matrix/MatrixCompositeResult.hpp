#include <memory>
#include <tuple>
#include <vector>
#include "../Is/Is.hpp"
#include "../String/Literal.hpp"
#include "../CSV/Elements.hpp"    

#pragma once

template<std::size_t, typename> class Matrix;

template<typename Q, typename M>
class IMatrixQueryResult
{
public:
	using QuantityType = Q;
	using MatrixType = M;
	virtual Q Value() const = 0;
private:
	friend 	std::ostream& operator<<(std::ostream& out, const IMatrixQueryResult& s) {	return s.display(out);	}
	virtual std::ostream& display(std::ostream& out)	const = 0;
};

template<typename Q, typename M>
class MatrixQueryResult: public IMatrixQueryResult<Q,M>
{
	using Base = IMatrixQueryResult<Q,M>;
public:
	MatrixQueryResult(const Q&& q, const M&& m = M(), const std::string& n =""): value{q}, item(m), name{n} {};
	MatrixQueryResult(std::shared_ptr<IElement> q, const M&& m = M(), const std::string& n =""): value{Q(q->Data())}, item(m), name{n} {};
	virtual Q Value() const { return value; }
private:
	friend 	std::ostream& operator<<(std::ostream& out, const MatrixQueryResult& s)	{	return out<<"Name: "<<s.name<<"\n"<<s.item<<"\nValue: "<<s.value;	}
	std::ostream& display(std::ostream& out) const { return out<<(*this); }
	typename Base::QuantityType value;
	M item;
	std::string name;
};

template<typename Q, typename M>
class MatrixCompositeQueryResult: public IMatrixQueryResult<Q,M>
{
	using Base = IMatrixQueryResult<Q,M>;
public:
	MatrixCompositeQueryResult(const Q&& q, std::unique_ptr<std::vector<std::unique_ptr<Base>>>&& v, const std::string& n =""): value{q}, items{std::move(v)},name{n} {};
	MatrixCompositeQueryResult(std::shared_ptr<IElement> q, std::unique_ptr<std::vector<std::unique_ptr<Base>>>&& v, const std::string& n =""): value{*q}, items{std::move(v)},name{n} {};
	virtual Q Value() const { return value; }
private:
	friend 	std::ostream& operator<<(std::ostream& out, const MatrixCompositeQueryResult& s)	
	{	
		out<<"Name: "<<s.name<<"\n";	
		std::for_each(s.items->cbegin(), s.items->cend(), [&out](const auto& i) { out<<*i<<"\n"; });
		return out<<"Value: "<<s.value;	
	}
	std::ostream& display(std::ostream& out) const { return out<<(*this); }
	typename Base::QuantityType value;
	std::unique_ptr<std::vector<std::unique_ptr<Base>>> items;
	std::string name;
};
