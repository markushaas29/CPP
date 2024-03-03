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
	auto operator()() const { return decorate(); }
private:
	friend std::ostream& operator<<(std::ostream& s, const IDecorator& c) { return c.display(s); }
	virtual std::ostream& display(std::ostream& s) const = 0;
	virtual std::unique_ptr<DecoratedType> decorate() const  = 0;
};

template<typename T, typename I>
class DecoratorBase: public IDecorator<T,I>
{
	using Base = IDecorator<T,I>;
	inline static constexpr const char TypeIdentifier[] = "DecoratorBase";
    inline static constexpr Literal TypeId{TypeIdentifier};
    template<typename U> using IsT =  Is<U,TypeId>;
public:
	DecoratorBase(const typename Base::DecoratedType& d): DecoratorBase{std::make_unique<typename Base::DecoratedType>(d)} {}
	DecoratorBase(std::unique_ptr<typename Base::DecoratedType> d): item{std::move(d)} {}
protected:
	std::unique_ptr<typename Base::DecoratedType> item;
};

template<typename T, typename I>
class Decorator: public DecoratorBase<T,I>
{
	using Base = DecoratorBase<T,I>;
	inline static constexpr const char TypeIdentifier[] = "Decorator";
    inline static constexpr Literal TypeId{TypeIdentifier};
    template<typename U> using IsT =  Is<U,TypeId>;
public:
	Decorator(const T& v, const typename Base::DecoratedType& d): Base{std::make_unique<typename Base::DecoratedType>(d)} {}
	Decorator(const T& v, std::unique_ptr<typename Base::DecoratedType> d): Base{std::move(d)} {}
private:
	T value;
	virtual std::unique_ptr<typename Base::DecoratedType> decorate() const  
	{ 
		auto result = Base::item->Units();
		result.push_back({"Test",{T::Identifier, value.ToString()}});
		return std::make_unique<FactoryUnitContainer<typename Base::DecoratedType::UnitType>>(Base::item->Name(),result); 
	};                                                                                                 
	virtual std::ostream& display(std::ostream& s) const { return	s<<"{"<<T::Identifier<<", "<<value<<"}";	}
};

template<typename T, typename I>
Decorator(const T&, const I&) -> Decorator<T,I>;

template<typename T, typename I>
Decorator(const T&, std::unique_ptr<I>) -> Decorator<T,I>;
