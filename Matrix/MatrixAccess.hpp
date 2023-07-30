#include <vector>
#include <initializer_list>
#include <memory>
#include <cassert>
#include "../Components/Interfaces.hpp"
#include "../Is/Is.hpp"
#include "../String/Literal.hpp"
#include "../String/Format.hpp"

#pragma once

template<typename M>
class MatrixAccess
{
public:
	MatrixAccess(M* ma): m{ma} {}
	decltype(auto) AddRow(const std::vector<typename M::IType>& v)
    {
        std::for_each(v.cbegin(), v.cend(), [&](auto i) { m->elements->push_back(std::make_shared<typename M::IType>(i)); } );
        typename M::IsT<Throwing>(Format("Not jagged: Size: ",v.size()))(!M::MI::checkJagged(v.size(),m->descriptor));
        m->descriptor.AddRow();
    }
    decltype(auto) Row(size_t i, const std::vector<typename M::DataType>& v, uint rows, uint cols) const
    {  
        assert(i<rows);
        std::vector<typename M::DataType> result;
        for(uint r = i * cols; r < (i+1) * cols; r++)
            result.push_back(v.at(r));
        return result;
    }
    decltype(auto) Col(size_t i, const std::vector<typename M::DataType>& v, uint rows, uint cols)
    {  
        assert(i<cols);
        std::vector<typename M::DataType> result;
        for(auto r = 0; r < rows; r++)
            result.push_back(v.at(i + (r * cols)));
        return result;
    }
private:
	M* m;
	friend M;
};
