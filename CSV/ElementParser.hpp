#include <tuple>
#include <memory>
#include "../String/Literal.hpp"
#include "../String/String_.hpp"

#pragma once

class ElementParser
{
protected:
	template<typename T>
	static std::unique_ptr<IElement> extract(const std::string& s, std::size_t& i)
	{
		auto end = i;
		i = s.find(T::Pattern);
		std::cout<<"FOUND: "<<i<<std::endl;
	  	if (i!=std::string::npos)
			return T::Extract(String_::Trim(std::string(s.begin()+i,s.begin()+end)));
		return nullptr;
	}
	template<size_t N>
	static void reg()
	{
		if constexpr (std::tuple_size<Elements>()==N)
			return;
		else
		{
			using Type = std::tuple_element_t<N,Elements>;
			std::cout<<Type::Identifier<<std::endl;
			//interface->Register(Type::Identifier,&Type::Make);
			reg<N+1>();
		}
	}
	
public:
	using Elements = std::tuple<BookingText,BIC,IBAN,Receiver>;
	inline static constexpr const char TypeIdentifier[] = "Element";
    inline static constexpr Literal LiteralType{TypeIdentifier};
	virtual std::vector<std::unique_ptr<IElement>> operator()(const std::string& s)
	{
		std::vector<std::unique_ptr<IElement>> v;
		
		auto i = s.size();
		v.push_back(extract<BookingText>(s,i));
		v.push_back(extract<BIC>(s,i));
		v.push_back(extract<IBAN>(s,i));
		v.push_back(extract<Receiver>(s,i));
		
		reg<0>();
		for(const auto& i : v)
			std::cout<<*i<<std::endl;

		return v;
	}
private:
	friend std::ostream& operator<<(std::ostream& out, const ElementParser& e) {	return out<<e;}
};

class ClientParser: public ElementParser
{
public:
	virtual std::vector<std::unique_ptr<IElement>> operator()(const std::string& s)
	{
		std::vector<std::unique_ptr<IElement>> v;
		
		auto i = s.size();
		v.push_back(extract<Client>(s,i));
		
		//reg<0>();
		for(const auto& i : v)
			std::cout<<*i<<std::endl;

		return v;
	}
};
