#include <vector>
#include <initializer_list>
#include <memory>
#include "../Components/Interfaces.hpp"
#include "../Is/Is.hpp"
#include "../String/Literal.hpp"
#include "../String/Format.hpp"

#pragma once
template<std::size_t, typename> class Matrix;

template<typename M>
class MatrixAccess
{
public:
	static constexpr size_t Order = M::Order;
	inline static constexpr const char TypeIdentifier[] = "MatrixImpl";
    inline static constexpr Literal LiteralType{TypeIdentifier};
	MatrixAccess() {}
private:
	friend M;
	template<typename T> using IsT = Is<T,LiteralType>;
	decltype(auto) addRow(const std::vector<typename M::ElementType>& v, M* m)
    {
        std::for_each(v.cbegin(), v.cend(), [&](auto i) { m->elements->push_back(std::make_shared<typename M::ElementType>(i)); } );
        typename M::IsT<Throwing>(Format("Not jagged: Size: ",v.size()))(!M::MI::checkJagged(v.size(),m->descriptor));
        m->descriptor.AddRow();
    }
    decltype(auto) row(size_t i, const M* m) const
    { 
		IsT<Throwing>(Format("Row Index: ",i, " exceeds row size: ", m->Rows()))(i<m->Rows());
        std::vector<typename M::DataType> result;
        for(uint r = i * m->descriptor.Stride(0); r < (i+1) * m->descriptor.Stride(0); r++)
            result.push_back(m->elements->at(r));
        return result;
    }
    decltype(auto) col(size_t i, const M* m)
    {  
		IsT<Throwing>(Format("Col Index: ",i, " exceeds col size: ", m->Cols()))(i<m->Cols());
        std::vector<typename M::DataType> result;
        for(auto r = 0; r < m->Rows(); r++)
            result.push_back(m->elements->at(i + (r * m->descriptor.Stride(0))));
        return result;
    }
	decltype(auto) slice(size_t i, const M* m) const 
	{
		if constexpr (Order==1)
			return *m;
		if constexpr (Order==2)
		{
			using MDT = MatrixDescriptor<Order-1, typename M::ElementType>;
			auto col = m->col(i);
			std::array<size_t,1> e {col.size()};
			return Matrix<Order-1, MDT>(MDT{e}, col);
		}
	}
	template<size_t N>
	decltype(auto) slices(std::array<size_t,N> arr, const M* m) const 
	{
		size_t max = *std::max_element(arr.begin(), arr.end());
		typename M::IsT<Throwing>(Format("Index: ",max ," exceeds extents!"))(max<m->Cols());
		using MDT = MatrixDescriptor<Order, typename M::ElementType>;
		std::vector<typename M::DataType> result;
		std::array<size_t,Order> e;
		std::array<size_t,Order> s;
		std::copy(m->descriptor.Extents().begin(), m->descriptor.Extents().end(), e.begin());
		std::copy(m->descriptor.Strides().begin(), m->descriptor.Strides().end(), s.begin());
		e[1] = arr.size();
		s[0] = arr.size();
		for(int i = 0; i < m->Rows(); ++i)
		{
			auto row = m->row(i);
			std::for_each(arr.begin(), arr.end(), [&](size_t i){ result.push_back(row[i]); });
		}
		
		return Matrix<Order, MDT>(MDT{e,s}, result);
	}
	decltype(auto) matrix(size_t i, const M* m) const 
	{
		using MDT = MatrixDescriptor<Order-1, typename M::ElementType>;
		std::array<size_t,Order-1> e;
		std::array<size_t,Order-1> s;
		std::copy(m->descriptor.Extents().begin()+1, m->descriptor.Extents().end(), e.begin());
		std::copy(m->descriptor.Strides().begin()+1, m->descriptor.Strides().end(), s.begin());
		auto row = m->row(i);
		if constexpr (Order-1==0)
			return MatrixElement<typename M::ElementType>(*(m->elements->at(i)));
		else
			return Matrix<Order-1, MDT>(MDT{e,s}, row);
	}
};
