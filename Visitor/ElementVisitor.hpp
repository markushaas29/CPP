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

template<typename T>
class ElementVisitor: public VariadicVisitor<void,T>, public BoolVisitable<bool>
{
	using ReturnType = void;
public:
	virtual std::unique_ptr<BaseVisitor> Copy() { return std::make_unique<ElementVisitor>(); };
	virtual ReturnType Visit(T& t) { elements.push_back(t.Clone()); };
	ElementVisitor() = default;
	ElementVisitor(ElementVisitor& e)	{	std::for_each(e.elements.cbegin(), e.elements.cend(), [&](const auto& i) { elements.push_back(i->Clone());});	}
protected:
	virtual bool Is(BaseVisitor& visitor) { return dynamic_cast<Visitor<T,ReturnType>*>(&visitor);  };
	
	auto&& get(auto&& res) 	
	{ 
		std::for_each(elements.cbegin(), elements.cend(), [&res](const auto& e) { res.push_back(e->Clone());});
		return res;	
	}
	auto& display(auto& s) 	
	{ 
		std::for_each(elements.cbegin(), elements.cend(), [&s](const auto& e) { s<<e<<"\n";});
		return s;	
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
	ElementCollector(ElementCollector& e)	{	std::for_each(e.elements.cbegin(), e.elements.cend(), [&](const auto& i) { elements.push_back(i->Clone());});	}
	auto Elements() { return elements = get(); }
	auto Size() { return elements.size(); }
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
		std::for_each(t.elements.cbegin(), t.elements.cend(), [&s](const auto& e) { s<<*e<<"\n";});
		return s;	
	}
};
