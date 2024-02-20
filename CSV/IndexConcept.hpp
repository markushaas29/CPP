#pragma once

class IElement;

//template<typename P> 
//concept IndexConcept = requires(P p)
//{
//	P::Identifier;
//    explicit P(const std::string&);
//	P::CreateIndexType(const std::string& s) const  { return std::make_unique<Type>(s); }
//};

template<typename T>
concept IndexConcept = requires(T t) 
{
		T::Identifier;
		T::IndexType;
};

