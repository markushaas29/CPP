#include <map>
#include <chrono>
#include <memory>
#include "../Is/Is.hpp"
#include "../String/Literal.hpp"

#pragma once

class Repository
{
public:
	inline static constexpr const char TypeIdentifier[] = "Repository";
	inline static constexpr Literal TypeId{TypeIdentifier};

	bool Is() { return false; }
private:
	friend std::ostream& operator<<(std::ostream& s, const Repository& c){return s;}
};
