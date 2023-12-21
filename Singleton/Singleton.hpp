#include <map>
#include <chrono>
#include <memory>
#include "../Is/Is.hpp"
#include "../String/Literal.hpp"

#pragma once

template<typename T>
class Singleton
{
public:
	inline static constexpr const char TypeIdentifier[] = "Singleton";
	inline static constexpr Literal TypeId{TypeIdentifier};

	static T& Get()
	{
		static T instance;
		return instance;
	}
private:
	friend std::ostream& operator<<(std::ostream& s, const Singleton& c){return s;}
	Singleton() { }
	~Singleton() { };
	Singleton& operator=(const Singleton&) = delete;
	Singleton(const Singleton& c) = delete;
};
