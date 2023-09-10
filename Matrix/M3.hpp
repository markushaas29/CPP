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
	explicit M3(const std::vector<MatrixType>& v):  elements{std::make_unique<std::vector<MatrixType>>(v.cbegin(), v.cend())}{  };

	M3& operator=(M3& m) { return M3(m.descriptor, std::vector<DataType>(m.elements->cbegin(),m.elements->cend()));}

	decltype(auto) Rows() const { return elements->size(); }
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
	template<typename T>
	decltype(auto) To() const { return access->template to<T>(this); }

//	template<typename F>
//	decltype(auto) Apply(F f) { return MC<Type>::apply(f, elements->cbegin(), elements->cend(), descriptor); }
private:
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

	std::unique_ptr<std::vector<MatrixType>> elements;
};
