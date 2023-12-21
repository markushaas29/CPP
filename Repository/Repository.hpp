#include <map>
#include <chrono>
#include <memory>
#include "../Is/Is.hpp"
#include "../String/Literal.hpp"

#pragma once

template<typename T>
class Repository
{
public:
	inline static constexpr const char TypeIdentifier[] = "Repository";
	inline static constexpr Literal TypeId{TypeIdentifier};

	static Repository& Get()
	{
		static Repository instance;
		return instance;
	}
private:
	friend std::ostream& operator<<(std::ostream& s, const Repository& c){return s;}
	Repository() { }
	~Repository() { };
	Repository& operator=(const Repository&) = delete;
	Repository(const Repository& c) = delete;
};
