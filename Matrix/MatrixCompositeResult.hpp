#include <memory>
#include <tuple>
#include <vector>
#include "../Is/Is.hpp"
#include "../String/Literal.hpp"
#include "../CSV/Elements.hpp"    

#pragma once

template<std::size_t, typename> class Matrix;

template<typename Q, typename MType>
class IMatrixQueryResult
{
public:
	using QuantityType = Q;
	using MatrixType = MType;
	using ElementMatrixType =  Matrix<1, MatrixDescriptor<1, std::shared_ptr<IElement>>>;
	virtual Q Value() const = 0;
	virtual ElementMatrixType Elements()
	{
		auto m = Init(this->elements());
		return m(); 
	};
	virtual MType M() const = 0;
private:
	virtual std::vector<std::shared_ptr<IElement>> elements() const = 0;
	friend 	std::ostream& operator<<(std::ostream& out, const IMatrixQueryResult& s) {	return s.display(out);	}
	virtual std::ostream& display(std::ostream& out)	const = 0;
};

template<typename Q, typename MType>
class MatrixQueryResult: public IMatrixQueryResult<Q,MType>
{
	using Base = IMatrixQueryResult<Q,MType>;
public:
	MatrixQueryResult(const Q&& q, const MType&& m = MType(), const std::string& n =""): value{q}, item(m), name{n} {};
	MatrixQueryResult(std::shared_ptr<IElement> q, const MType&& m = MType(), const std::string& n =""): value{Q(q->Data())}, item(m), name{n} {};
	virtual Q Value() const { return value; }
	virtual MType M() const { return item; };
private:
	friend 	std::ostream& operator<<(std::ostream& out, const MatrixQueryResult& s)	{	return out<<"Name: "<<s.name<<"\n"<<s.item<<"\nValue: "<<s.value;	}
	std::ostream& display(std::ostream& out) const { return out<<(*this); }
	virtual std::vector<std::shared_ptr<IElement>> elements() const	{	return std::vector<std::shared_ptr<IElement>>{ std::make_shared<Q>(value) };	};
	typename Base::QuantityType value;
	MType item;
	std::string name;
};

template<typename Q, typename MType>
class MatrixCompositeQueryResult: public IMatrixQueryResult<Q,MType>
{
	using Base = IMatrixQueryResult<Q,MType>;
public:
	MatrixCompositeQueryResult(const Q&& q, std::unique_ptr<std::vector<std::unique_ptr<Base>>>&& v, const std::string& n =""): value{q}, items{std::move(v)},name{n} {};
	MatrixCompositeQueryResult(std::shared_ptr<IElement> q, std::unique_ptr<std::vector<std::unique_ptr<Base>>>&& v, const std::string& n =""): value{*q}, items{std::move(v)},name{n} {};
	virtual Q Value() const { return value; }
	virtual MType M() const { return MType(); };
	virtual typename Base::ElementMatrixType Elements() const 
	{
		std::vector<std::shared_ptr<IElement>> v;
		std::for_each(items->cbegin(), items->cend(), [&v](const auto& i) { v.push_back(std::make_shared<Q>(i->Value())); });
		auto m = Init(v);
		return m(); 
	};
private:
	friend 	std::ostream& operator<<(std::ostream& out, const MatrixCompositeQueryResult& s)	
	{	
		out<<"Name: "<<s.name<<"\n";	
		std::for_each(s.items->cbegin(), s.items->cend(), [&out](const auto& i) { out<<*i<<"\n"; });
		return out<<"Value: "<<s.value;	
	}
	virtual std::vector<std::shared_ptr<IElement>> elements() const
	{
		std::vector<std::shared_ptr<IElement>> v;
		std::for_each(items->cbegin(), items->cend(), [&v](const auto& i) { v.push_back(std::make_shared<Q>(i->Value())); });
		return v; 
	};
	std::ostream& display(std::ostream& out) const { return out<<(*this); }
	typename Base::QuantityType value;
	std::unique_ptr<std::vector<std::unique_ptr<Base>>> items;
	std::string name;
};
