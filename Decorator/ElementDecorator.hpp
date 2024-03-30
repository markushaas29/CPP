#include "../ObjectFactory/FactoryUnit.hpp" 
#include "../String/Literal.hpp" 
#include "../Is/Is.hpp" 

#pragma once

class IElementDecorator
{
public:
	auto operator()(const std::string& s) const { return decorate(s); }
private:
	//friend std::ostream& operator<<(std::ostream& s, const IDecorator& c) { return c.display(s); }
	virtual std::string decorate(const std::string s) const  = 0;
};

class Bold: public IElementDecorator
{
	inline static constexpr const char TypeIdentifier[] = "Bold";
    inline static constexpr Literal TypeId{TypeIdentifier};
    template<typename U> using IsT =  Is<U,TypeId>;
public:
//	Bold(const T& v, const typename Base::DecoratedType& d): Base{std::make_unique<typename Base::DecoratedType>(d)} {}
//	Bold(const T& v, std::unique_ptr<typename Base::DecoratedType> d): Base{std::move(d)} {}
private:
	virtual std::string decorate(const std::string s) const { return "<b>"+s+"<b>"; }
};
