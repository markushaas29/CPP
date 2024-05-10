#include <sstream>
#include <vector> 
#include "MatrixReader.hpp"

#pragma once

class MatrixBuilder
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
