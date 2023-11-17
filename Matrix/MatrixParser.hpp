#include <vector>
#include <initializer_list>
#include <memory>
#include "PointerConcept.hpp"
#include "../Is/Is.hpp"
#include "../To/To.hpp"
#include "../CSV/Matcher.hpp"
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
    inline static constexpr Literal LiteralType{TypeIdentifier};
	using PointerType = std::shared_ptr<IElement>;
	using DescriptorType = MatrixDescriptor<Order,PointerType>;
	MatrixParser() {}
private:
	friend M;
    static decltype(auto) parse(const M* m, const Matcher& matcher) 
    { 
		auto el = std::vector<PointerType>();
        std::for_each(m->elements->cbegin(), m->elements->cend(), [&](const auto& e) { el.push_back(std::make_shared<Quantity<Sum>>(*e)); });
        //std::for_each(m->elements->cbegin(), m->elements->cend(), [&](const auto& e) { std::cout<<(*matcher.match(*e)[0])<<std::endl; });

		auto d = DescriptorType(m->descriptor.Extents(), m->descriptor.Strides());
		return Matrix<Order,DescriptorType>(d,ToDataType(el));
    }
};
