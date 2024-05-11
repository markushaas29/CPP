#include <sstream>
#include <vector> 
#include "MatrixReader.hpp"

#pragma once

template<size_t N> class MatrixBuilder;


template<typename T> 
class MatrixBuilderBase
{
public:
    auto operator()() const  {  return Init(v)(); }
	auto& AddRow() 
	{ 
		v.push_back(T()); 
		return *this;
	}
protected:
	std::vector<T> v;
};

template<>
class MatrixBuilder<1>: public MatrixBuilderBase<std::string>
{
public:

	auto& Add(const auto& val) 
	{
		std::stringstream ss;
		ss<<val;
		v.push_back(ss.str());
		return *this;
	}
};

template<>
class MatrixBuilder<2>: public MatrixBuilderBase<std::vector<std::string>>
{
public:
	auto& Add(const auto& val) 
	{
		if(v.size()==0)
			v.push_back(std::vector<std::string>());
		std::stringstream ss;
		v.back().push_back(ss.str());
		return *this;
	}
};
