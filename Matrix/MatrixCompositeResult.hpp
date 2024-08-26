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
	virtual Q Value() const = 0;
	virtual MType M() const = 0;
	virtual const std::string& Name() const = 0;
	virtual std::ofstream& operator()(std::ofstream& s) const = 0;
	decltype(auto) Elements() {	return Init(elements())(); 	};
	decltype(auto) Funcs()	{	return Init(funcs())(); };
	decltype(auto) FuncVec() { return funcs(); }
private:
	friend 	std::ostream& operator<<(std::ostream& out, const IResult& s) {	return s.display(out);	}
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
		std::for_each(items->cbegin(), items->cend(), [](const auto& i) 
				{
//					auto html = HtmlBuilder(i->Name()+".html","/home/markus/Downloads/CSV_TestFiles_2");
//					html(i->M());
					auto mf1 = MatrixFormatter(i->M());
     
					auto v = i->M().Elements();
					std::cout<<"Size: "<<v.size()<<std::endl;
     				auto htmlMF = mf1.Html();
					//std::cout<<*htmlMF<<std::endl;
				});
		return items->at(0)->M(); 
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
	std::ostream& display(std::ostream& out) const { return out<<(*this); }
	typename Base::QuantityType value;
	std::unique_ptr<std::vector<std::unique_ptr<Base>>> items;
	std::string name;
};
