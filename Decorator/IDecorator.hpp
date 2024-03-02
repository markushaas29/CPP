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
	auto operator()(const auto& v) 
	{ 
		VecType result(v.begin(), v.end());
		result.push_back(decorate());
		return result;
	}
private:
	friend std::ostream& operator<<(std::ostream& s, const IDecorator& c) { return c.display(s); }
	virtual std::ostream& display(std::ostream& s) const = 0;
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
	virtual typename Base::DecoratedType decorate() const  { return value.ToString(); };                                                                                                 
	virtual std::ostream& display(std::ostream& s) const { return	s<<"{"<<T::Identifier<<", "<<value<<"}";	}
};

template<typename T>
Decorator(const T&) -> Decorator<T>;
