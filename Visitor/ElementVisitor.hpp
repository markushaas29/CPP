 #include "Visitor.hpp"
 #include "../CSV/Element.hpp"
 #include "../CSV/Elements.hpp"
 #include "../Common/Date.hpp"
 #include "../Common/UniqueCast.hpp"
 #include "../Unit/Unit.hpp"
 #include "../Functional/Functional.hpp"
 #include "../Quantity/Quantity.hpp"
 #include "../Quantity/QuantityRatio.hpp"

#pragma once 
class Date;
class IBAN;
class IElement;
class Name;
template<typename,typename, typename> class Quantity;

template<typename L, typename R, template<typename, typename> class FT>
class FuncVisitorBase
{
public:
	using ReturnType = void;
	template<typename T> using C = Constant<T>;
	using Op = FT<C<L>,C<R>>;
	decltype(auto) operator()() { return Op{left, right}(); }
	decltype(auto) F() { return Op{left, right}; }
protected:
	C<L> left = C{L{}};
	C<R> right= C{R{}};
private:
	friend std::ostream& operator<<(std::ostream& s, const FuncVisitorBase& f) 	{ return s<<Op{f.left, f.right};	}
};

template<typename L, typename R, template<typename, typename> class FT>
class FuncVisitor: public FuncVisitorBase<L,R,FT>, public virtual VariadicVisitor<void, L,R>
{
	using Base = FuncVisitorBase<L,R,FT>;
	template<typename T> using C = Constant<T>;
public:
	virtual Base::ReturnType Visit(L& l) { Base::left = C{l}; };
	virtual Base::ReturnType Visit(R& r) { Base::right = C{r}; };
	virtual std::unique_ptr<BaseVisitor> Copy() { return std::make_unique<FuncVisitor>(); };
};

template<typename L, template<typename, typename> class FT>
class FuncVisitor<L,L,FT>: public FuncVisitorBase<L,L,FT>, public virtual VariadicVisitor<void, L>
{
	using Base = FuncVisitorBase<L,L,FT>;
	template<typename T> using C = Constant<T>;
public:
	virtual Base::ReturnType Visit(L& l)	
	{ 
		leftSet ? ( Base::right = C{l} ) : ( Base::left = C{l} ) ;
		leftSet = true; 
	};
	virtual std::unique_ptr<BaseVisitor> Copy() { return std::make_unique<FuncVisitor>(); };
private:
	bool leftSet = false;
};

template<typename L, typename B, template<typename, typename> class FT>
class ComposedFuncVisitor: public virtual FuncVisitorBase<L,L,FT>, public virtual VariadicVisitor<void, L>, public virtual B
{
	using ReturnType = void;
	using Base = FuncVisitorBase<L,L,FT>;
	template<typename T> using C = Constant<T>;
	using Op = FT<C<L>,typename B::Op>;
public:
	decltype(auto) operator()() { return Op{Base::left, B::F()}(); }
	decltype(auto) F() { return Op{Base::left, B::F()}; }
	virtual ReturnType Visit(L& l) 	{ Base::left = C{l}; };
	virtual std::unique_ptr<BaseVisitor> Copy() { return std::make_unique<ComposedFuncVisitor>(); };
private:
	//friend std::ostream& operator<<(std::ostream& s, const ComposedFuncVisitor& f) 	{ return s<<Op{ComposedFuncVisitor.left, B::F()};	}
};

template<typename T>
class ElementVisitor: public VariadicVisitor<void,T>
{
	using ReturnType = void;
public:
	virtual std::unique_ptr<BaseVisitor> Copy() { return std::make_unique<ElementVisitor>(); };
	virtual ReturnType Visit(T& t) { elements.push_back(t.Clone()); };
	ElementVisitor() = default;
	ElementVisitor(ElementVisitor& e)
	{
		std::for_each(e.elements.cbegin(), e.elements.cend(), [&](const auto& i) { elements.push_back(i->Clone());});
	}
protected:
	auto&& get(auto&& res) 	
	{ 
		std::for_each(elements.cbegin(), elements.cend(), [&res](const auto& e) { res.push_back(e->Clone());});
		return res;	
	}
private:
	std::vector<std::shared_ptr<IElement>> elements;
};

template<typename... Types>
class ElementCollector: public ElementVisitor<Types>...
{
	using ReturnType = void;
	using Tup = std::tuple<Types...>;
public:
	ElementCollector(){}
	ElementCollector(ElementCollector& e)
	{
		std::for_each(e.elements.cbegin(), e.elements.cend(), [&](const auto& i) { elements.push_back(i->Clone());});
	}
	auto operator()() { elements = get(); }
	template<typename T>
	auto To() const
	{
//		if constexpr (std::is_same_v<T,Quantity<Sum,Pure,double>>)
//			return sum;
//		else if constexpr (std::is_same_v<T,IBAN>)
//			return iban;
//		else if constexpr (std::is_same_v<T,Date>)
//			return date;
	}
private:
	std::vector<std::shared_ptr<IElement>> elements;
	virtual std::unique_ptr<BaseVisitor> Copy() { return std::make_unique<ElementCollector>(); };
	auto get() 	
	{ 
		std::vector<std::shared_ptr<IElement>> r;
		return exec<0>(r);	
	}
	template<size_t N>
	auto exec(auto&& res)
	{
		if constexpr (std::tuple_size<Tup>()==N)
			return std::move(res);
		else
		{
			using Type = std::tuple_element_t<N,Tup>;
			res = ElementVisitor<Type>::get(res);
			return exec<N+1>(res);
		}
	}
	
	
	friend std::ostream& operator<<(std::ostream& s, const ElementCollector& t) 	
	{ 
		std::for_each(t.elements.cbegin(), t.elements.cend(), [&s](const auto& e) { s<<*e;});
		return s;	
	}
};

template<typename... Types>
class TransferVisitor: public VariadicVisitor<void, Types...>, public BoolVisitable<bool>
{
	using ReturnType = void;
public:
	using PtrType = std::vector<std::shared_ptr<IElement>>;
	using SumType = Quantity<Sum,Pure,double>;
	inline static constexpr size_t Order = 4;
	auto I() const { return iban; }
	template<typename T>
	auto To() const
	{
		if constexpr (std::is_same_v<T,Quantity<Sum,Pure,double>>)
			return sum;
		else if constexpr (std::is_same_v<T,IBAN>)
			return iban;
		else if constexpr (std::is_same_v<T,Date>)
			return date;
	}
	auto Create() const	{	return std::vector<std::shared_ptr<IElement>> {std::make_shared<IBAN>(iban), std::make_shared<Quantity<Sum,Pure,double>>(sum), std::make_shared<Date>(date.D(), date.M(), date.Y()), std::make_shared<Entry>(entry) };	}
	virtual ReturnType Visit(Quantity<Sum,Pure,double>& q) { sum = sum + q; };
	virtual ReturnType Visit(Date& d) { date = d;  };
	virtual ReturnType Visit(IBAN& i) { iban = i; };
	virtual ReturnType Visit(Entry& i) { entry = i; };
	virtual std::unique_ptr<BaseVisitor> Copy() { return std::make_unique<TransferVisitor>(); };
	virtual bool Is(BaseVisitor& visitor) 
	{
		std::vector<std::shared_ptr<IElement>> v = { std::make_shared<SumType>(sum), std::make_shared<IBAN>(iban), std::make_shared<Date>(date), std::make_shared<Entry>(entry) };
		for(auto p : v)
			if(p->Is(visitor))
				return true;
		return false; 
	};
private:
	SumType sum;
	IBAN iban;
	Date date;
	Entry entry;
	friend std::ostream& operator<<(std::ostream& s, const TransferVisitor& t) 	{ return s<<"IBAN: "<<t.iban<<"Date: "<<t.date<<"Sum: "<<t.sum;	}
};
