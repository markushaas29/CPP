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

template<typename D = std::string>
class MatrixReader
{
public:
	inline static constexpr const char TypeIdentifier[] = "MatrixInitializer";
    inline static constexpr Literal LiteralType{TypeIdentifier};
	using CSVSeparator = T::char_<';'> ;
	using DoubleSeparator = T::char_<'.'> ;
	using Type = D;
	using M1Type = MatrixInitializer<1,Type>;
	using M2Type = MatrixInitializer<2,Type>;
	using VariantType = std::variant<M1Type,M2Type>;
	MatrixReader(const std::string& s):info{std::make_unique<FS::FileInfo>(fs::path{s})}, matrix{execute(s)} {}
	template<size_t N>
	decltype(auto) M() { return std::get<N-1>(matrix).Get(); }
	template<size_t N>
	decltype(auto) Is() {	return N==order;	}
	decltype(auto) Order() { return order; }
	decltype(auto) operator()(const std::string& s) { return execute(s); }
private:
	VariantType matrix;
	size_t order;
	std::unique_ptr<FS::FileInfo> info;
	friend std::ostream& operator<<(std::ostream& s, const MatrixReader& i) { return s<<"Order: "<<i.order<<"\n"<<i.info->Path();  }
	
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
			if constexpr (std::is_same_v<Type,std::string>)
			{
				if(String_::Contains(d, std::string(1,CSVSeparator::Value)))
				{
					auto v = String_::Split(d,CSVSeparator::Value);
					vec.insert(vec.end(),v.begin(),v.end());
					auto m = Init(process2(vec, std::move(is)));
					return m;
				}
				else
				{
					vec.push_back(d);
					if(iss.peek() == CSVSeparator::Value) 
        				iss.ignore();
				}
			}
			else
			{
				vec.push_back(d);
				if(iss.peek() == CSVSeparator::Value) 
        			iss.ignore();
			}
		}

				std::cout<<"VS "<<vec.size()<<"\n";
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

		order = 1;

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
				if constexpr (std::is_same_v<Type,std::string>)
				{
					if(String_::Contains(d, std::string(1,CSVSeparator::Value)))
						v = String_::Split(d,CSVSeparator::Value);
					else
						v.push_back(d);
				}
	   			else
				{
					v.push_back(d);
	   				if(iss.peek() == CSVSeparator::Value) 
           				iss.ignore();
				}
	   		}

			for(auto s : v)
				std::cout<<"S "<<s<<v.size()<<"\n";

			result.push_back(v);
	   	}

		order = 2;
		
		return result;
	}	
};
