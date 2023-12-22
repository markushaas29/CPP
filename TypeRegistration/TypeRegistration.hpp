#include <map>
#include <chrono>
#include <memory>
#include "../Is/Is.hpp"
#include "../String/Literal.hpp"

#pragma once

template<typename... T>
class TypeRegistratiom
{
public:
	inline static constexpr const char TypeIdentifier[] = "TypeRegistratiom";
	inline static constexpr Literal TypeId{TypeIdentifier};

	bool Is() { return false; }
private:
	friend std::ostream& operator<<(std::ostream& s, const TypeRegistratiom& c){return s;}
};
