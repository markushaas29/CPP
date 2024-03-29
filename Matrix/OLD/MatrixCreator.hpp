#include <memory>
#include <vector>
#include <array>
#include "Matrix.hpp" 
#include "../File/Node.hpp" 
#include "../String/String_.hpp" 
#include "../Wrapper/Wrapper.hpp" 

#pragma once

template<size_t N, typename> class MatrixImpl;
template<size_t, typename> class MatrixDescriptor;
class CreatorBase
{
public:
	virtual void Create(const std::string&) = 0;
	//friend std::ostream& operator<<(std::ostream& s, const MatrixCreator& i) { return s<<"Size: "<<i.size<<"\tStart: "<<i.start;  }
};

template<typename MT>
class MatrixCreator
{
public:
	using Type = MT;
	using DescriptorType = typename MT::DescriptorType;
	using IType = typename MT::ElementType;
	using DataType = typename MT::DataType;
	using CSVSeparator = T::char_<';'> ;
	void Create(const std::string& s) 
	{
		auto path = fs::path{ "/home/markus/Downloads/CSV_TestFiles_2/U_2022.csv" };
        auto fi = std::make_shared<FS::FileInfo>(path);
        auto f = FS::File(fi);
		auto lines = f.Read();
		uint rows = 0;
		std::vector<DataType> data;
		std::for_each(lines.cbegin(), lines.cend(), [&](const auto& l)
				{
					auto vals = String_::Split<CSVSeparator>(l);
					rows = vals.size();
					std::for_each(vals.cbegin(), vals.cend(), [&](const auto& v) { data.push_back(std::make_shared<IType>(v)); });
				});

		std::array<std::size_t,2> e { lines.size(), rows };
		auto desc = DescriptorType{e};
		m = Type(desc,data);
	}

	Type&& Get() { return std::move(m); } 
private:
	Type m{};
	friend std::ostream& operator<<(std::ostream& s, const MatrixCreator& i) { return s<<"Size: "<<i.size<<"\tStart: "<<i.start;  }
};
