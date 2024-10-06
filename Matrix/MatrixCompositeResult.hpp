#include <memory>
#include <tuple>
#include <vector>
#include "../Is/Is.hpp"
#include "../String/Literal.hpp"
#include "../CSV/Elements.hpp"    
#include "../Matrix/MatrixFormatter.hpp"
#include "../Functional/Functional.hpp"    
#include "../HTMLBuilder/HTMLBuilder.hpp"    

#pragma once

template<std::size_t, typename> class Matrix;

template<typename Q, typename MType>
class IResult
{
public:
	using QuantityType = Q;
	using FuncType = Acc<Q>;
	using MatrixType = MType;
	using MatrixOne =  Matrix<1, MatrixDescriptor<1,std::shared_ptr<IElement>>>;
	virtual Q Value() const = 0;
	virtual MType M() const = 0;
	auto Names() const { return names(); };
	virtual const std::string& Name() const = 0;
	virtual std::ofstream& operator()(std::ofstream& s) const = 0;
	decltype(auto) Elements() {	return Init(elements())(); 	};
	decltype(auto) Funcs()	{	return Init(funcs())(); };
	decltype(auto) FuncVec() { return funcs(); }
private:
	friend 	std::ostream& operator<<(std::ostream& out, const IResult& s) {	return s.display(out);	}
	virtual MatrixOne names() const = 0;
	virtual std::vector<std::shared_ptr<IElement>> elements() const = 0;
	virtual std::vector<FuncType> funcs() const = 0;
	virtual std::ostream& display(std::ostream& out)	const = 0;
};

template<typename Q, typename MType>
class Result: public IResult<Q,MType>
{
	using Base = IResult<Q,MType>;
public:
	Result(const typename Base::FuncType&& q, const MType&& m = MType(), const std::string& n =""): value{q()}, item(m), name{n}, result{q} {};
	Result(const Q&& q, const MType&& m = MType(), const std::string& n =""): value{q}, item(m), name{n} {};
	virtual Q Value() const { return Q{result()}; }
	virtual MType M() const { return item; };
	virtual const std::string& Name() const { return name; };
	virtual std::ofstream& operator()(std::ofstream& s) const 
	{ 
		auto mf = MatrixFormatter(item);  
		return HtmlBuilder()(s,mf());
	};
private:
	friend 	std::ostream& operator<<(std::ostream& out, const Result& s)	{	return out<<"Name: "<<s.name<<"\n"<<s.item<<"\nValue: "<<s.value<<s.result;	}
	std::ostream& display(std::ostream& out) const { return out<<(*this); }
	virtual std::vector<std::shared_ptr<IElement>> elements() const	{	return std::vector<std::shared_ptr<IElement>>{ std::make_shared<Q>(result()) };	};
	virtual std::vector<typename Base::FuncType> funcs() const { return {result};};
	virtual typename Base::MatrixOne names() const 
	{
		std::vector<std::shared_ptr<IElement>> res = { std::make_shared<Header>(name) };
		return Init(res)();
	};
	typename Base::QuantityType value;
	MType item;
	std::string name;
	typename Base::FuncType result;
};

template<typename Q, typename MType>
class CompositeResult: public IResult<Q,MType>
{
	using Base = IResult<Q,MType>;
public:
	CompositeResult(const Q&& q, std::unique_ptr<std::vector<std::unique_ptr<Base>>>&& v, const std::string& n =""): value{q}, items{std::move(v)},name{n} {};
	CompositeResult(std::shared_ptr<IElement> q, std::unique_ptr<std::vector<std::unique_ptr<Base>>>&& v, const std::string& n =""): value{*q}, items{std::move(v)},name{n} {};
	virtual Q Value() const { return value; }
	virtual const std::string& Name() const { return name; };
	virtual MType M() const 
	{ 
		using DT = MType::DescriptorType;
		std::vector<std::shared_ptr<IElement>> res;
		std::for_each(items->cbegin(), items->cend(), [&res](const auto& i) 
				{
					auto mf1 = MatrixFormatter(i->M());
     
					auto m = i->M();
					auto v = m.Elements();
					if(v.size()!=0)
					{
						v.push_back(std::make_shared<Entry>(i->Name()));
						v.push_back(std::make_shared<Entry>("-"));
						v.push_back(std::make_shared<Entry>("-"));
						v.push_back(std::make_shared<Entry>("-"));
						v.push_back(i->Value().Clone());
						res.insert(res.end(),v.begin(), v.end());
					}
					});
		size_t cols = 5;
		if(res.size()!=0)
		{
			while(res.size()%5 != 0)
				res.push_back(std::make_shared<Entry>("-"));
				return MType(DT({res.size()/ cols,cols}), res);
		}
		return MType();
	};
	decltype(auto) Elements() const 
	{
		std::vector<std::shared_ptr<IElement>> v;
		std::for_each(items->cbegin(), items->cend(), [&v](const auto& i) { v.push_back(std::make_shared<Q>(i->Value())); });
		auto m = Init(v);
		return m(); 
	};
	virtual std::ofstream& operator()(std::ofstream& s) const 
	{	
		std::for_each(items->cbegin(), items->cend(), [&s](const auto& i) {	(*i)(s);	});
		return s;	
	}
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
	virtual std::vector<typename Base::FuncType> funcs() const
	{
		std::vector<typename Base::FuncType> v;
		std::for_each(items->cbegin(), items->cend(), [&v](const auto& i) 
				{
					auto fs = i->FuncVec();
					v.insert(v.end(), fs.begin(),fs.end());
				});
		return v; 
	};
	virtual typename Base::MatrixOne names() const 
	{
		std::vector<std::shared_ptr<IElement>> res;
		std::for_each(items->cbegin(), items->cend(), [&res](const auto& i) {	res.push_back(std::make_shared<Header>(i->Name()));		});
		return Init(res)();
	};
	std::ostream& display(std::ostream& out) const { return out<<(*this); }
	typename Base::QuantityType value;
	std::unique_ptr<std::vector<std::unique_ptr<Base>>> items;
	std::string name;
};
