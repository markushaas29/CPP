#include <vector>
#include <initializer_list>
#include <memory>
#include "PointerConcept.hpp"
#include "../Is/Is.hpp"
#include "../To/To.hpp"
#include "../CSV/Matcher.hpp"
#include "../CSV/Elements.hpp"
#include "../String/Literal.hpp"
#include "../String/Format.hpp"

#pragma once
template<std::size_t, typename> class Matrix;

template<typename M>
class MatrixParser
{
public:
	static constexpr size_t Order = M::Order;
	inline static constexpr const char TypeIdentifier[] = "MatrixParser";
    inline static constexpr Literal TypeId{TypeIdentifier};
	using PointerType = std::shared_ptr<IElement>;
	using DescriptorType = MatrixDescriptor<Order,PointerType>;
	MatrixParser() {}
private:
	friend M;
    
	static decltype(auto) parse(const M* m, const Matcher& matcher) 
    { 
		if constexpr (std::is_same_v<typename M::ElementType, std::string>)
		{
			auto el = std::vector<PointerType>();
	        std::for_each(m->elements->cbegin(), m->elements->cend(), [&](const auto& e) 
					{ 
						if(matcher.Match(*e))
							el.push_back(matcher(*e)); 
						else
							el.push_back(std::make_shared<Entry>(*e)); 
					});
	
			IsT<Throwing>(Format("Parsed elements: ",el.size(), " matrix elements: ", m->elements->size() ))(el.size()==m->elements->size());
	
			auto d = DescriptorType(m->descriptor.Extents(), m->descriptor.Strides());
			return Matrix<Order,DescriptorType>(d,ToDataType(el));
		}
    }
    
	static decltype(auto) match(const M* m, const Matcher& matcher) 
    { 
		auto cols = std::vector<size_t>();
        size_t c{};
		for(auto i = m->elements->cbegin(); i != m->elements->cend() && c < m->Cols(); ++i, ++c) 
			if(matcher.Match(**i))
				cols.push_back(c); 

		return m->Cols(cols);
    }

	template<typename U> using IsT =  Is<U,TypeId>;
};
