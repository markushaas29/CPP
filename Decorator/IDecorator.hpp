#include "../ObjectFactory/FactoryUnit.hpp" 
#include "../String/Literal.hpp" 
#include "../Is/Is.hpp" 

#pragma once

template<typename T, typename DT = std::string>
class IDecorator
{
public:
	using ValueType = T;
	using DecoratedType = DT;
	using VecType = std::vector<DecoratedType>;
//	auto operator()(const auto& v) 
//	{ 
//		VecType result(v.begin(), v.end());
//		result.push_back(decorate());
//		return result;
//	}
private:
	friend std::ostream& operator<<(std::ostream& s, const IDecorator& c) { return c.display(s); }
	virtual std::ostream& display(std::ostream& s) const = 0;
//	virtual DecoratedType decorate() const = 0;                                                                                                 
};

template<typename T, typename I>
class Decorator: public IDecorator<T,I>
{
	using Base = IDecorator<T,I>;
	inline static constexpr const char TypeIdentifier[] = "Decorator";
    inline static constexpr Literal TypeId{TypeIdentifier};
    template<typename U> using IsT =  Is<U,TypeId>;
public:
	Decorator(const T& v, const typename Base::DecoratedType& d): value{v}, item{d} {}
private:
	T value;
	typename Base::DecoratedType item;
	//virtual typename Base::DecoratedType decorate() const  { return value.ToString(); };                                                                                                 
	virtual std::ostream& display(std::ostream& s) const { return	s<<"{"<<T::Identifier<<", "<<value<<"}";	}
};

template<typename T, typename I>
Decorator(const T&, const I&) -> Decorator<T,I>;

