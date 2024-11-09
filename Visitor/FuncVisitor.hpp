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
	decltype(auto) F() const { return Op{left, right}; }
protected:
	C<L> left = C{L{}};
	C<R> right= C{R{}};
private:
	virtual std::ostream& display(std::ostream& s) const{ return s<<Op{left, right};	}
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
	decltype(auto) operator()() const { return Op{Base::left, B::F()}(); }
	decltype(auto) F() const { return Op{Base::left, B::F()}; }
	virtual ReturnType Visit(L& l) 	{ Base::left = C{l}; };
	virtual std::unique_ptr<BaseVisitor> Copy() { return std::make_unique<ComposedFuncVisitor>(); };
private:
	virtual std::ostream& display(std::ostream& s) const{ return s<<F();	}
};
