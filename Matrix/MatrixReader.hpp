#include <memory>
#include <vector>
#include <array>
#include <variant>
#include "Matrix.hpp" 
#include "MatrixInitializer.hpp" 
#include "../File/Node.hpp" 
#include "../String/String_.hpp" 
#include "../Wrapper/Wrapper.hpp" 

#pragma once

template<size_t N, typename> class MatrixImpl;
template<size_t, typename> class MatrixDescriptor;
class ReaderBase
{
public:
	virtual void Create(const std::string&) = 0;
	//friend std::ostream& operator<<(std::ostream& s, const MatrixReader& i) { return s<<"Size: "<<i.size<<"\tStart: "<<i.start;  }
};

template<typename D = std::string>
class MatrixReader
{
public:
	using CSVSeparator = T::char_<';'> ;
	using DoubleSeparator = T::char_<'.'> ;
	using Type = D;
	using M1Type = MatrixInitializer<1,Type>;
	using M2Type = MatrixInitializer<2,Type>;
	using VariantType = std::variant<M1Type,M2Type>;
	MatrixReader(const std::string& s):info{std::make_unique<FS::FileInfo>(fs::path{s})}, matrix{execute(s)} {}
	decltype(auto) operator ()() { return execute(); }
private:
	VariantType matrix;
	std::unique_ptr<FS::FileInfo> info;
	friend std::ostream& operator<<(std::ostream& s, const MatrixReader& i) { return s<<i.info->Path();  }
	
	VariantType execute(const std::string& s)
	{
		std::string line;
		Type d;
		std::vector<Type> vec;
		auto is = std::make_unique<std::ifstream>(s);
		getline (*is,line); 
		std::istringstream iss{line};
		while(iss)
		{
			iss>>d;
			vec.push_back(d);
			if(iss.peek() == CSVSeparator::Value) 
        		iss.ignore();
		}

		if(vec.size()>1)
		{
			auto m = Init(process2(vec, std::move(is)));
			return m;
		}

		auto m = Init(process1(vec, std::move(is)));
		return m;
	}

	decltype(auto) process1(std::vector<Type>& vec, auto is)
	{
		std::string line;
		Type d;

	   	while ( getline (*is,line) )
		{	
			std::istringstream iss{line};
   			iss>>d;
   			vec.push_back(d);
	   	}

		return vec;
	}

	decltype(auto) process2(const std::vector<Type>& vec, auto is)
	{
		std::string line;
		Type d;

		std::vector<std::vector<Type>> result{vec};
	   	while ( getline (*is,line) )
		{
			std::vector<Type> v;
			std::istringstream iss{line};
			while(iss)
	   		{	
	   			iss>>d;
	   			v.push_back(d);
	   			if(iss.peek() == CSVSeparator::Value) 
           			iss.ignore();
	   		}

			result.push_back(v);
	   	}
		
		return result;
	}	
};
