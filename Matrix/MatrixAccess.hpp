#include <vector>
#include <initializer_list>
#include <memory>
#include <cassert>
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
	MatrixAccess() {}
private:
	friend M;
	decltype(auto) addRow(const std::vector<typename M::IType>& v, M* m)
    {
        std::for_each(v.cbegin(), v.cend(), [&](auto i) { m->elements->push_back(std::make_shared<typename M::IType>(i)); } );
        typename M::IsT<Throwing>(Format("Not jagged: Size: ",v.size()))(!M::MI::checkJagged(v.size(),m->descriptor));
        m->descriptor.AddRow();
    }
    decltype(auto) row(size_t i, const M* m) const
    {  
        assert(i<m->Rows());
        std::vector<typename M::DataType> result;
        for(uint r = i * m->Cols(); r < (i+1) * m->Cols(); r++)
            result.push_back(m->elements->at(r));
        return result;
    }
    decltype(auto) col(size_t i, const M* m)
    {  
        assert(i<m->Cols());
        std::vector<typename M::DataType> result;
        for(auto r = 0; r < m->Rows(); r++)
            result.push_back(m->elements->at(i + (r * m->Cols())));
        return result;
    }
	decltype(auto) matrix(size_t i, const M* m) const 
	{
		using MDT = MatrixDescriptor<Order-1, typename M::IType>;
		std::array<size_t,Order-1> e;
		std::array<size_t,Order-1> s;
		std::copy(m->descriptor.Extents().begin()+1, m->descriptor.Extents().end(), e.begin());
		std::copy(m->descriptor.Strides().begin()+1, m->descriptor.Strides().end(), s.begin());
		auto row = m->Row(i);
		if constexpr (Order-1==0)
			return MatrixElement<typename M::IType>(*(m->elements->at(i)));
		else
			return Matrix<Order-1, MDT>(MDT{e,s}, row);
	}
};
