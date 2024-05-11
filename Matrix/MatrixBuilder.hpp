#include <sstream>
#include <vector> 
#include "MatrixReader.hpp"

#pragma once

template<size_t N> class MatrixBuilder;

template<>
class MatrixBuilder<1>
{
public:
    auto operator()() const
    {
        return Init(v)();                                                                                                       
    }

	auto& Add(const auto& val) 
	{
		std::stringstream ss;
		ss<<val;
		v.push_back(ss.str());
		return *this;
	}
private:
	std::vector<std::string> v;
};

template<>
class MatrixBuilder<2>
{
public:
    auto operator()() const
    {
        return Init(v)();                                                                                                       
    }

	auto& Add(const auto& val) 
	{
		std::stringstream ss;
		v.back().push_back(ss.str());
		return *this;
	}
private:
	std::vector<std::vector<std::string>> v;
};
