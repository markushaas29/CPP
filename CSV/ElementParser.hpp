#include <tuple>
#include <memory>
#include "../String/Literal.hpp"
#include "../String/String_.hpp"

#pragma once

class IElementParser
{
public:
	auto operator()(const std::string& s) const { return handle(s); };
	auto Is(const std::string& s) const { return handles(s); };
private:
	friend std::ostream& operator<<(std::ostream& out, const IElementParser& e) {	return out<<e;}
	virtual std::vector<std::unique_ptr<IElement>> handle(const std::string& s) const = 0;
	virtual bool handles(const std::string& s) const = 0;
};

template<typename D>
class ElementParser: public IElementParser
{
protected:
	template<typename T>
	static std::unique_ptr<IElement> extract(const std::string& s, std::size_t& i, char splitter = ':')
	{
		auto end = i;
		i = s.find(T::Pattern);
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
private:
	std::string token = "";
	friend std::ostream& operator<<(std::ostream& out, const ElementParser& e) {	return out<<e;}
	virtual bool handles(const std::string& s) const { return s.starts_with(token); };
};


class ReceiverParser: public ElementParser<ReceiverParser>
{
	inline static constexpr const char* Token = "Empfänger";
	virtual std::vector<std::unique_ptr<IElement>> handle(const std::string& s) const
	{
		std::vector<std::unique_ptr<IElement>> v;
		
		auto i = s.size();
		v.push_back(extract<Ref>(s,i));
		v.push_back(extract<BookingText>(s,i));
		v.push_back(extract<BIC>(s,i));
		v.push_back(extract<IBAN>(s,i));
		v.push_back(extract<Receiver>(s,i));
		
		reg<0>();
		for(const auto& i : v)
			std::cout<<*i<<std::endl;

		return v;
	}
};

class ClientParser: public ElementParser<ClientParser>
{
	inline static constexpr const char* Token = "Auftraggeber";
	virtual std::vector<std::unique_ptr<IElement>> handle(const std::string& s) const
	{
		std::vector<std::unique_ptr<IElement>> v;
		
		auto i = s.size();
		v.push_back(extract<Ref>(s,i));
		v.push_back(extract<BookingText>(s,i));
		v.push_back(std::make_unique<BIC>("GENODE61DET"));
		v.push_back(std::make_unique<BIC>("GENODE61DET"));
		v.push_back(extract<Client>(s,i));
		
		//reg<0>();
		for(const auto& i : v)
			std::cout<<*i<<std::endl;

		return v;
	}
};
