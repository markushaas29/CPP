#include <vector>
#include <initializer_list>
#include <memory>
#include "MatrixDescriptor.hpp"
#include "MatrixElement.hpp"
#include "MatrixInitializer.hpp"
#include "MatrixStrategy.hpp"
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

	MatrixType operator[] (size_t i) const 
	{
		IsT<Throwing>(Format("Index: ",i, " exceeds extents ", Rows()))(i < Rows());
		return elements->at(i); 
	}

	decltype(auto) Cols(auto... v) const { return apply<T>([&](auto& mx, const auto& e) { mx.push_back(e.Cols(v...)); }); } 
	template<typename VT>
	decltype(auto) Cols(const VT& v) const { return apply<T>([&](auto& mx, const auto& e) { mx.push_back(e.Cols(v)); }); } 

	decltype(auto) Parse(const Matcher& m) const { return apply<std::shared_ptr<IElement>>([&](auto& mx, const auto& e) { mx.push_back(e.Parse(m)); }); }
	decltype(auto) Match(const Matcher& m) const { return apply<T>([&](auto& mx, const auto& e) { mx.push_back(e.Match(m)); }); }
	template<typename TO>
	decltype(auto) To() const { return apply<TO>([&](auto& mx, const auto& e) { mx.push_back(e.template To<TO>()); }); }
	template<typename V>
   	decltype(auto) Accept(const V& visitors) const { return apply<std::shared_ptr<IElement>>([&](auto& mx, const auto& e) { mx.push_back(e.Accept(visitors)); }); }
	template<typename V>
   	decltype(auto) operator |(V v) const { return apply<std::shared_ptr<IElement>>([&](auto& mx, const auto& e) { mx.push_back((e | v)); }); }
	template<typename V>
   	decltype(auto) operator |(std::unique_ptr<V> v) const {	return (*this) | std::shared_ptr<V>(std::move(v)); }

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

	template<typename Q, typename U>
	decltype(auto) M(const IMatrixStrategy<MatrixType, Q, U>& s) 
	{ 
		std::vector<DataType> result;
		for(auto el : *elements)
		{
			auto m = s(el);
			std::cout<<"M3 "<<m<<std::endl;
			auto item = m.Items();
			for(auto i = 0; i < item.Rows(); ++i)
			{
				auto row = item.row(i);
				std::for_each(row.cbegin(), row.cend(), [&](const auto& v) { result.push_back(v); });
			}
		}

		std::array<size_t,Order-1> e;
		std::copy(elements->at(0).descriptor.Extents().begin(), elements->at(0).descriptor.Extents().end(), e.begin());
		e[0] = result.size() / e[1];

		auto resM = MatrixType(typename MatrixType::DescriptorType{e}, result); 
		return typename IMatrixStrategy<MatrixType, Q, U>::ResultType(Quantity<Sum>(resM.ColSum(11)), resM, s.Units(),std::string(s.Name())); 
	}
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

	template<typename C, typename F>
	decltype(auto) apply(F f) const 
	{ 
		std::vector<typename MatrixInitializer<2,C>::MatrixType> mx;
		std::for_each(elements->cbegin(), elements->cend(), [&](const auto& e) { f(mx,e); });
		return M3<C,DT>(mx); 
	}

	decltype(auto) getCols()
	{
		IsT<Throwing>(Format("No Data!"))(elements->size()>0);
		size_t cs = elements->at(0).Cols();
		std::for_each(elements->cbegin(), elements->cend(), [&](const auto& m) { IsT<Throwing>(Format("Cols ",m.Cols()," doesnt fit ",cs))(cs == m.Cols()); });
		return cs;
	};
};
