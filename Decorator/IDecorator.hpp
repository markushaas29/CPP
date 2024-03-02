#include "../String/Literal.hpp" 
#include "../Is/Is.hpp" 

#pragma once

template<typename T>
class IDecorator
{
public:
	using ValueType = T;
	using DecoratedType = std::string;
	using VecType = std::vector<DecoratedType>;
	decltype(auto) operator()(const VecType& v) const { return v.push_back(decorate()); }
private:
	//friend std::ostream& operator<<(std::ostream& s, const Decorator& c) 
	//{
	//	s<<"{";
	//	for(size_t i= 0;i < c.value.size(); ++i)
	//		s<<c.value[i]<<(i < (c.value.size()-1) ? Derived::sign : "");
	//	return s<<"}";  
	//}
	virtual DecoratedType decorate() const = 0;                                                                                                 
};

template<typename T>
class Decorator: public IDecorator<T>
{
	using Base = IDecorator<T>;
	inline static constexpr const char TypeIdentifier[] = "Decorator";
    inline static constexpr Literal TypeId{TypeIdentifier};
    template<typename U> using IsT =  Is<U,TypeId>;
public:
	Decorator(const T& v): value{v} {}
private:
	T value;
	virtual typename Base::DecoratedType decorate() const  { return value; };                                                                                                 
};

template<typename T>
Decorator(const T&) -> Decorator<T>;
