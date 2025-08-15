#include "../String/Literal.hpp"

#pragma once

class ElementParser
{
	inline static constexpr const char TypeIdentifier[] = "Element";
    inline static constexpr Literal LiteralType{TypeIdentifier};
public:

private:
	friend std::ostream& operator<<(std::ostream& out, const ElementParser& e) {	return out<<e;}
};

