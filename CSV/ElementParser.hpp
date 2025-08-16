#include <tuple>
#include <memory>
#include "../String/Literal.hpp"
#include "../String/String_.hpp"

#pragma once

class ElementParser
{
	using Elements = std::tuple<BookingText,BIC,IBAN,Receiver>;
	inline static constexpr const char TypeIdentifier[] = "Element";
    inline static constexpr Literal LiteralType{TypeIdentifier};
	template<typename T>
	static std::unique_ptr<IElement> extract(const std::string& s, std::size_t& i)
	{
		auto end = i;
		i = s.find(T::Pattern);
	  	if (i!=std::string::npos)
			return std::make_unique<T>(String_::Trim(std::string(s.begin()+i,s.begin()+end)));
		return nullptr;
	}
public:
	auto operator()(const std::string& s)
	{
		std::vector<std::unique_ptr<IElement>> v;
		
		auto i = s.size();
		v.push_back(extract<BookingText>(s,i));
		v.push_back(extract<BIC>(s,i));
		v.push_back(extract<IBAN>(s,i));
		v.push_back(extract<Receiver>(s,i));

		for(const auto& i : v)
			std::cout<<*i<<std::endl;

		return v;
	}
private:
	friend std::ostream& operator<<(std::ostream& out, const ElementParser& e) {	return out<<e;}
};

