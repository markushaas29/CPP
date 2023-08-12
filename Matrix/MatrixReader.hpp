#include <memory>
#include <vector>
#include <array>
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
	MatrixReader(const std::string& s):info{std::make_unique<FS::FileInfo>(fs::path{s})}, is{std::make_unique<std::ifstream>(s)} {}
	decltype(auto) E()
	{
		std::string line;
		Type d;
		std::vector<Type> vec;
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
			auto m = Init(process2(vec));
			std::cout<<"S "<<2<<"  "<<decltype(m)::Order<<"\n";
			std::cout<<m.Get()<<"\n";
			return;
		}

		auto m = Init(process1(vec));
		std::cout<<"S "<<1<<"  "<<decltype(m)::Order<<"\n";
		std::cout<<m.Get()<<"\n";
	}
//	void Create(const std::string& s) 
//	{
//		auto path = fs::path{ "/home/markus/Downloads/CSV_TestFiles_2/U_2022.csv" };
//        auto fi = std::make_shared<FS::FileInfo>(path);
//        auto f = FS::File(fi);
//		auto lines = f.Read();
//		uint rows = 0;
//		std::vector<DataType> data;
//		std::for_each(lines.cbegin(), lines.cend(), [&](const auto& l)
//				{
//					auto vals = String_::Split<CSVSeparator>(l);
//					rows = vals.size();
//					std::for_each(vals.cbegin(), vals.cend(), [&](const auto& v) { data.push_back(std::make_shared<IType>(v)); });
//				});
//
//		std::array<std::size_t,2> e { lines.size(), rows };
//		auto desc = DescriptorType{e};
//		m = Type(desc,data);
//	}
//
//	Type&& Get() { return std::move(m); } 
private:
	std::unique_ptr<std::ifstream> is;
	std::unique_ptr<FS::FileInfo> info;
	friend std::ostream& operator<<(std::ostream& s, const MatrixReader& i) { return s<<i.info->Path();  }
	decltype(auto) process1(std::vector<Type> vec)
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
	decltype(auto) process2(std::vector<Type> vec)
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
