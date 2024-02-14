#include <memory>
#include <tuple>
#include <vector>
#include "../Is/Is.hpp"
#include "../String/Literal.hpp"
#include "../CSV/Elements.hpp"    
#include "../Functional/Functional.hpp"    

#pragma once

template<std::size_t, typename> class Matrix;

template<typename Q, typename MType>
class IResult
{
public:
	using QuantityType = Q;
	using FuncType = Acc<Q>;
	using MatrixType = MType;
	using ElementMatrixType =  Matrix<1, MatrixDescriptor<1, std::shared_ptr<IElement>>>;
	using FuncMatrixType =  Matrix<1, MatrixDescriptor<1, Acc<Quantity<Unit<1>>>>>;
	virtual Q Value() const = 0;
	virtual ElementMatrixType Elements()
	{
		auto m = Init(this->elements());
		return m(); 
	};
	FuncMatrixType F()
	{
		auto m = Init(this->funcs());
		return m(); 
	};
	virtual MType M() const = 0;
	decltype(auto) FS() { return funcs(); }
private:
	virtual std::vector<std::shared_ptr<IElement>> elements() const = 0;
	virtual std::vector<Acc<Quantity<Unit<1>>>> funcs() const = 0;
	friend 	std::ostream& operator<<(std::ostream& out, const IResult& s) {	return s.display(out);	}
	virtual std::ostream& display(std::ostream& out)	const = 0;
};

template<typename Q, typename MType>
class Result: public IResult<Q,MType>
{
	using Base = IResult<Q,MType>;
public:
	Result(const Acc<Quantity<Unit<1> >>&& q, const MType&& m = MType(), const std::string& n =""): value{q()}, item(m), name{n}, result{q} {};
	Result(const Q&& q, const MType&& m = MType(), const std::string& n =""): value{q}, item(m), name{n} {};
	Result(std::shared_ptr<IElement> q, const MType&& m = MType(), const std::string& n =""): value{Q(q->Data())}, item(m), name{n} {};
	virtual Q Value() const { return value; }
	virtual MType M() const { return item; };
private:
	friend 	std::ostream& operator<<(std::ostream& out, const Result& s)	{	return out<<"Name: "<<s.name<<"\n"<<s.item<<"\nValue: "<<s.value<<s.result;	}
	std::ostream& display(std::ostream& out) const { return out<<(*this); }
	virtual std::vector<std::shared_ptr<IElement>> elements() const	{	return std::vector<std::shared_ptr<IElement>>{ std::make_shared<Q>(result()) };	};
	virtual std::vector<Acc<Quantity<Unit<1>>>> funcs() const { return {result};};
	typename Base::QuantityType value;
	MType item;
	std::string name;
	Acc<Quantity<Unit<1> >> result;
};

template<typename Q, typename MType>
class CompositeResult: public IResult<Q,MType>
{
	using Base = IResult<Q,MType>;
public:
	CompositeResult(const Q&& q, std::unique_ptr<std::vector<std::unique_ptr<Base>>>&& v, const std::string& n =""): value{q}, items{std::move(v)},name{n} {};
	CompositeResult(std::shared_ptr<IElement> q, std::unique_ptr<std::vector<std::unique_ptr<Base>>>&& v, const std::string& n =""): value{*q}, items{std::move(v)},name{n} {};
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
	friend 	std::ostream& operator<<(std::ostream& out, const CompositeResult& s)	
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
	virtual std::vector<Acc<Quantity<Unit<1>>>> funcs() const
	{
		std::vector<Acc<Quantity<Unit<1>>>> v;
		std::for_each(items->cbegin(), items->cend(), [&v](const auto& i) 
				{
					auto fs = i->FS();
					//std::for_each(fs.cbegin(), fs.cend(), [&v](const auto& f) { v.push_back(f);}); 
					v.insert(v.end(), fs.begin(),fs.end());
				});
		return v; 
	};
	std::ostream& display(std::ostream& out) const { return out<<(*this); }
	typename Base::QuantityType value;
	std::unique_ptr<std::vector<std::unique_ptr<Base>>> items;
	std::string name;
};
