#include <vector>
#include <initializer_list>
#include <memory>
#include "MatrixDescriptor.hpp"
#include "MatrixElement.hpp"
#include "MatrixCalculator.hpp"
#include "MatrixInitializer.hpp"
#include "../Calculator/CalculatorResult.hpp"
#include "../Calculator/Operations.hpp"
#include "../Is/Is.hpp"
#include "../String/Literal.hpp"

#pragma once

template<typename T = std::string, typename DT=MatrixDescriptor<3,T>>
class M3
{
public:
	inline static constexpr const char TypeIdentifier[] = "M3";
	inline static constexpr Literal LiteralType{TypeIdentifier};
	static constexpr size_t Order = 3;
	using Type = M3<T,DT>;
	using DescriptorType = DT;
	using ElementType = typename DT::ElementType;
	using DataType = typename DT::DataType;
	using ValueType = ElementType;
	using VecType = std::vector<std::vector<std::vector<ElementType>>>;
	using MatrixType = MatrixInitializer<2,T>::MatrixType;

	M3() = delete;
	M3(M3&&) = default;
	M3& operator=(M3&&) = default;
	~M3() = default;

	explicit M3(const VecType& v): M3(init(v)){  };
	explicit M3(const std::vector<MatrixType>& v):  elements{std::make_unique<std::vector<MatrixType>>(v.cbegin(), v.cend())}, cols{getCols()} {}

	M3& operator=(M3& m) { return M3(m.descriptor, std::vector<DataType>(m.elements->cbegin(),m.elements->cend()));}

	decltype(auto) Rows() const { return elements->size(); }
	decltype(auto) Cols() const { return cols; }
	decltype(auto) Descriptor(size_t i)const { return (*this)[i].Descriptor(); }

	MatrixType operator[] (size_t i) const 
	{
		IsT<Throwing>(Format("Index: ",i, " exceeds extents ", Rows()))(i < Rows());
		return elements->at(i); 
	}
//	decltype(auto) AddRow(const std::vector<ElementType>& v) { access->addRow(v,this); }
//	decltype(auto) Col(size_t i) const { return access->colAt(i, this); }
//	decltype(auto) Cols(auto... i) const { return access->cols(std::array<size_t,sizeof...(i)>{size_t(i)...}, this); }
//	decltype(auto) Rows(auto... i) const { return access->rows(std::array<size_t,sizeof...(i)>{size_t(i)...}, this); }
	template<typename TO>
	decltype(auto) To() const 
	{
		std::vector<typename MatrixInitializer<2,TO>::MatrixType> mx;
		std::for_each(elements->cbegin(), elements->cend(), [&mx](const auto& v2) { mx.push_back( v2.template To<TO>() ); });
		return M3<TO>(mx); 
	}

	template<typename ET = ElementType>
	decltype(auto) M(const IMatrixQuery<MatrixType, ET>& query) 
	{ 
		std::vector<DataType> result;
		for(auto el : *elements)
		{
			auto m = el.M(query);
			for(auto i = 0; i < m.Rows(); ++i)
			{
				auto row = m.row(i);
				std::for_each(row.cbegin(), row.cend(), [&](const auto& v) { result.push_back(v); });
			}
		}

		std::array<size_t,Order-1> e;
		std::copy(elements->at(0).descriptor.Extents().begin(), elements->at(0).descriptor.Extents().end(), e.begin());
		e[0] = result.size() / e[1];

		return MatrixType(typename MatrixType::DescriptorType{e}, result); 
	}

//	template<typename F>
//	decltype(auto) Apply(F f) { return MC<Type>::apply(f, elements->cbegin(), elements->cend(), descriptor); }
private:
	std::unique_ptr<std::vector<MatrixType>> elements;
	size_t cols;
	template<typename U> using IsT =  Is<U,LiteralType>;
	decltype(auto) init (const VecType& v)  
	{
		std::vector<MatrixType> ms;
		std::for_each(v.cbegin(), v.cend(), [&ms](const auto& v2) { ms.push_back(Init(v2)()); });
		return ms; 
	}

	friend std::ostream& operator<<(std::ostream& s, const M3& m) 
	{ 
		s<<"{";
			for(auto i=0; i<m.Rows(); ++i)
				s<<m.elements->at(i)<<(i+1 == m.Rows()? "" : ", ");
		return s<<" }";
	}

	decltype(auto) getCols()
	{
		IsT<Throwing>(Format("No Data!"))(elements->size()>0);
		size_t cs = elements->at(0).Cols();
		std::for_each(elements->cbegin(), elements->cend(), [&](const auto& m) { IsT<Throwing>(Format("Cols ",m.Cols()," doesnt fit ",cs))(cs == m.Cols()); });
		return cs;
	};
};
