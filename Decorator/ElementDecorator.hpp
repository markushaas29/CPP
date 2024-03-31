#include "../ObjectFactory/FactoryUnit.hpp" 
#include "../String/Literal.hpp" 
#include "../Is/Is.hpp" 

#pragma once

class IElementDecorator
{
public:
	auto operator()(const std::string& s) const { return "<" + decorate() + "/>" + s +"<" + decorate() + "/>"; }
private:
	//friend std::ostream& operator<<(std::ostream& s, const IDecorator& c) { return c.display(s); }
	virtual std::string decorate() const  = 0;
};

class HtmlDecorator: public IElementDecorator
{
	inline static constexpr const char TypeIdentifier[] = "HtmlDecorator";
    inline static constexpr Literal TypeId{TypeIdentifier};
    template<typename U> using IsT =  Is<U,TypeId>;
private:
	virtual std::string decorate() const { return "b"; }
};
class Bold: public IElementDecorator
{
	inline static constexpr const char TypeIdentifier[] = "Bold";
    inline static constexpr Literal TypeId{TypeIdentifier};
    template<typename U> using IsT =  Is<U,TypeId>;
private:
	virtual std::string decorate() const { return "b"; }
};
