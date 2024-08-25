#include <vector>
#include <initializer_list>
#include <memory>
#include "PointerConcept.hpp"
#include "../Functional/FunctionalConcept.hpp"
#include "../Components/Interfaces.hpp"
#include "../Is/Is.hpp"
#include "../CSV/Token.hpp"
#include "../CSV/Matcher.hpp"
#include "../To/To.hpp"
#include "../String/Literal.hpp"
#include "../String/Format.hpp"

#pragma once
template<std::size_t, typename> class Matrix;

template<typename M>
class MatrixAccess
{
public:
	static constexpr size_t Order = M::Order;
	inline static constexpr const char TypeIdentifier[] = "MatrixAccess";
    inline static constexpr Literal LiteralType{TypeIdentifier};
	MatrixAccess() {}
private:
	friend M;
	template<typename T> using IsT = Is<T,LiteralType>;
	decltype(auto) set(M* m, typename M::ElementType e, auto... I)
    {
		static_assert(sizeof...(I) == Order, "Arguments do not mtach");
		std::vector<typename M::DataType> v;
	    auto i = M::MI::computePosition(m->descriptor.Extents(),m->descriptor.Strides(), I...);
		std::for_each(m->elements->begin(), m->elements->end(),[&v](const auto& i) { v.push_back(std::make_shared<typename M::ElementType>(*i)); } );
      	v.at(i) = std::make_shared<typename M::ElementType>(e); 

		return M(m->descriptor,v); 
    }
	decltype(auto) clone(const M* m) const
    {
		if constexpr(std::is_same_v<typename M::ElementType, std::shared_ptr<IElement>>)
		{
			std::vector<typename M::DataType> v;
			std::for_each(m->elements->begin(), m->elements->end(),[&v](const auto& i) { v.push_back(std::make_shared<typename M::ElementType>((*i)->Clone())); } );
			return v; 
		}
    }
	decltype(auto) exec(M* m)
    {
		if constexpr(std::is_invocable_v<typename M::ValueType>)
		{
			using RT = decltype((*(m->elements->at(0)))());
			using MDT = MatrixDescriptor<Order, RT>;
			std::array<size_t,Order> e;
			std::copy(m->descriptor.Extents().begin(), m->descriptor.Extents().end(), e.begin());
			std::vector<std::shared_ptr<RT>> v;
			std::for_each(m->elements->begin(), m->elements->end(),[&v](const auto& i) { v.push_back(std::make_shared<RT>((*i)())); } );
			return Matrix<Order, MDT>(MDT{e}, v);
		}
		else
			return *m;
    }
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
	decltype(auto) colAt(size_t i, const M* m) const 
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
	template<typename... I>
	decltype(auto) cols(const M* m, I... i) const 
	{
		using Index = std::tuple_element_t<0, std::tuple<I...>>;
		if constexpr (!std::is_same_v<Index,std::string>)
			return colsByIndex(std::array<size_t,sizeof...(i)>{size_t(i)...}, m);
		else
			return colsByKey(std::array<std::string,sizeof...(i)>{i...}, m);
	}
	template<size_t N>
	decltype(auto) colsByKey(std::array<std::string,N> arr, const M* m) const 
	{
		std::array<size_t,N> arri;
		for(uint i = 0; i < arr.size(); ++i)
		{
			auto it = std::find_if(m->elements->cbegin(), m->elements->cend(), [&](const auto& v) { return arr[i] == (*v)->Data(); });
			if(it != m->elements->cend() )
				arri[i] = it - m->elements->cbegin();
		}
		return colsByIndex(arri, m);
	}
	template<size_t N>
	decltype(auto) colsByIndex(std::array<size_t,N> arr, const M* m) const 
	{
		size_t max = *std::max_element(arr.begin(), arr.end());
		typename M::IsT<Throwing>(Format("Index: ",max ," exceeds extents!"))(max<m->Cols());
		using MDT = MatrixDescriptor<Order, typename M::ElementType>;
		std::vector<typename M::DataType> result;
		std::array<size_t,Order> e;
		std::copy(m->descriptor.Extents().begin(), m->descriptor.Extents().end(), e.begin());
		e[1] = arr.size();
		for(int i = 0; i < m->Rows(); ++i)
		{
			auto row = m->row(i);
			std::for_each(arr.begin(), arr.end(), [&](size_t i){ result.push_back(row[i]); });
		}
		
		return Matrix<Order, MDT>(MDT{e}, result);
	}
	template<typename T>
	decltype(auto) cols(const std::vector<T>& v, const M* m) const 
	{
		if constexpr (std::is_same_v<typename M::ElementType, std::string> && std::is_same_v<T, std::unique_ptr<IToken>>)
		{
			auto vi = std::make_unique<std::vector<std::unique_ptr<IToken>>>();
			Matcher imatcher(v);
        
			auto vs = std::vector<std::string>();
			std::for_each(m->elements->cbegin(),m->elements->cbegin() + m->Cols(), [&](auto& s) { vs.push_back(*s); });
        	auto imatches = imatcher.Indices(vs);
			auto vst = std::vector<size_t>();
			std::for_each(imatches.cbegin(),imatches.cend(), [&](auto& m) { vst.push_back(m->Id()); });

			return cols(vst,m);
		}
		else
		{
			size_t max = *std::max_element(v.begin(), v.end());
			typename M::IsT<Throwing>(Format("Index: ",max ," exceeds extents!"))(max<m->Cols());
			using MDT = MatrixDescriptor<Order, typename M::ElementType>;
			std::vector<typename M::DataType> result;
			std::array<size_t,Order> e;
			std::copy(m->descriptor.Extents().begin(), m->descriptor.Extents().end(), e.begin());
			e[1] = v.size();
			for(int i = 0; i < m->Rows(); ++i)
			{
				auto row = m->row(i);
				std::for_each(v.begin(), v.end(), [&](size_t i){ result.push_back(row[i]); });
			}
			
			return Matrix<Order, MDT>(MDT{e}, result);
		}
	}
	template<size_t N>
	decltype(auto) rows(std::array<size_t,N> arr, const M* m) const 
	{
		size_t max = *std::max_element(arr.begin(), arr.end());
		typename M::IsT<Throwing>(Format("Index: ",max ," exceeds extents!"))(max<m->Rows());
		using MDT = MatrixDescriptor<Order, typename M::ElementType>;
		std::vector<typename M::DataType> result;
		std::array<size_t,Order> e;
		std::copy(m->descriptor.Extents().begin(), m->descriptor.Extents().end(), e.begin());
		e[0] = arr.size();
		for(int i = 0; i < arr.size(); ++i)
		{
			auto row = m->row(arr[i]);
			std::for_each(row.begin(), row.end(), [&](auto i){ result.push_back(i); });
		}
		
		return Matrix<Order, MDT>(MDT{e}, result);
	}
	decltype(auto) sub(std::array<size_t,2> rowSpan, std::array<size_t,2> colSpan,const M* m) const 
	{
		size_t maxR = *std::max_element(rowSpan.begin(), rowSpan.end());
		typename M::IsT<Throwing>(Format("Index: ",maxR ," exceeds extents!"))(maxR<m->Rows());
		size_t maxC = *std::max_element(colSpan.begin(), colSpan.end());
		typename M::IsT<Throwing>(Format("Index: ",maxC ," exceeds extents!"))(maxC<m->Cols());
		using MDT = MatrixDescriptor<Order, typename M::ElementType>;
		std::vector<typename M::DataType> result;
		std::array<size_t,Order> e;
		
		if(rowSpan[1] == 0)
			rowSpan[1] = m->Rows() - 1;
		e[0] = rowSpan[1] - rowSpan[0] + 1;
			
		if(colSpan[1] == 0)
			colSpan[1] = m->Cols() - 1;
		e[1] = colSpan[1] - colSpan[0] + 1;
		MDT mdt{e};
		
		for(int i = rowSpan[0]; i <= rowSpan[1] ; ++i)
		{
			auto row = m->row(i);
			for(int i = colSpan[0]; i <= colSpan[1] ; ++i)
				result.push_back(row[i]);
		}

		return Matrix<Order, MDT>(mdt, result);
	}
	decltype(auto) slices(std::vector<size_t> rows, std::vector<size_t> cols,const M* m) const 
	{
		size_t maxR = *std::max_element(rows.begin(), rows.end());
		typename M::IsT<Throwing>(Format("Index: ",maxR ," exceeds extents!"))(maxR<m->Rows());
		size_t maxC = *std::max_element(cols.begin(), cols.end());
		typename M::IsT<Throwing>(Format("Index: ",maxC ," exceeds extents!"))(maxC<m->Cols());
		using MDT = MatrixDescriptor<Order, typename M::ElementType>;
		std::vector<typename M::DataType> result;
		std::array<size_t,Order> e;
		std::copy(m->descriptor.Extents().begin(), m->descriptor.Extents().end(), e.begin());
		e[0] = rows.size();
		e[1] = cols.size();
		for(int i = 0; i < rows.size(); ++i)
		{
			auto row = m->row(rows[i]);
			std::for_each(cols.begin(), cols.end(), [&](size_t j){	result.push_back(row[j]); });
		}

		return Matrix<Order, MDT>(MDT{e}, result);
	}
	decltype(auto) matrix(size_t i, const M* m) const 
	{
		using MDT = MatrixDescriptor<Order-1, typename M::ElementType>;
		std::array<size_t,Order-1> e;
		std::array<size_t,Order-1> s;
		std::copy(m->descriptor.Extents().begin()+1, m->descriptor.Extents().end(), e.begin());
		auto row = m->row(i);
		if constexpr (Order-1==0)
			return MatrixElement<typename M::ElementType>(*(m->elements->at(i)));
		else
			return Matrix<Order-1, MDT>(MDT{e}, row);
	}
	template<typename T>
	decltype(auto) to(const M* m) const 
	{
		using MDT = MatrixDescriptor<Order, T>;
		std::array<size_t,Order> e;
		std::array<size_t,Order> s;
		std::copy(m->descriptor.Extents().begin(), m->descriptor.Extents().end(), e.begin());
		std::vector<typename MDT::DataType> result;
		if constexpr (std::is_same_v<typename M::ElementType, std::string>)
			std::for_each(m->elements->cbegin(), m->elements->cend(), [&result](auto e){ result.push_back(std::make_shared<T>(To<T>(*e))); });
		else if constexpr (std::is_same_v<T, std::string>)
			std::for_each(m->elements->cbegin(), m->elements->cend(), [&result](auto e){ result.push_back(std::make_shared<T>(std::to_string(*e))); });
		else if constexpr (PointerConcept<typename M::ElementType>)
			std::for_each(m->elements->cbegin(), m->elements->cend(), [&result](auto e)
					{
						if constexpr (std::is_same_v<typename M::ElementType, std::shared_ptr<IElement>>)
							result.push_back(std::make_shared<T>((*e)->template To<T>())); 
						else
    						if (auto ptr = std::dynamic_pointer_cast<T>(*e))
								result.push_back(std::make_shared<T>(*ptr)); 
							else
								result.push_back(std::make_shared<T>("")); 
					});
		else
			std::for_each(m->elements->cbegin(), m->elements->cend(), [&result](auto e){ result.push_back(std::make_shared<T>(static_cast<T>(*e))); });

		return Matrix<Order, MDT>(MDT{e}, result);
	}
};
