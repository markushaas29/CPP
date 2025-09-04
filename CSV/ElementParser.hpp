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

template<typename D, typename... TP>
class ElementParser: public IElementParser
{
protected:
	using Elements = std::tuple<TP...>;
	static inline constexpr size_t Num = std::tuple_size<Elements>();
	template<typename T>
	static std::unique_ptr<IElement> extractElement(const std::string& s, std::size_t& i, char splitter = ':')
	{
		auto end = i;
		i = s.find(T::Pattern);
	  	if (i!=std::string::npos)
			return T::Extract(String_::Trim(std::string(s.begin()+i,s.begin()+end)));
		return T::Make(s);
	}
	template<size_t N>
	static void extractElements(const std::string& s, std::size_t is, std::vector<std::unique_ptr<IElement>>& v)
	{
		if constexpr (0==N)
			return;
		else
		{
			using Type = std::tuple_element_t<N-1,Elements>;
			auto i = s.find(Type::Pattern);
	  		if (i!=std::string::npos)
				v.push_back(extractElement<Type>(s,is));
			else
				v.push_back(std::make_unique<Empty>("--"));
			extractElements<N-1>(s,is,v);
		}
	}
public:
	ElementParser(): token{D::Token} {}
	inline static constexpr const char TypeIdentifier[] = "Element";
    inline static constexpr Literal LiteralType{TypeIdentifier};
private:
	std::string token = "";
	friend std::ostream& operator<<(std::ostream& out, const ElementParser& e) {	return out<<e;}
	virtual bool handles(const std::string& s) const { return s.starts_with(token); };
};


class ReceiverParser: public ElementParser<ReceiverParser,Receiver,IBAN, BIC, BookingText, Ref>
{
	friend class ElementParser<ReceiverParser,Receiver,IBAN, BIC, BookingText, Ref>;
	inline static constexpr const char* Token = "Empf";
	virtual std::vector<std::unique_ptr<IElement>> handle(const std::string& s) const
	{
		std::vector<std::unique_ptr<IElement>> v;
		extractElements<Num>(s,s.size(),v);
		return v;
	}
};

class ClientParser: public ElementParser<ClientParser,Client,BookingText,Ref>
{
	friend class ElementParser<ClientParser,Client,BookingText,Ref>;
	inline static constexpr const char* Token = "Auftraggeber";
	virtual std::vector<std::unique_ptr<IElement>> handle(const std::string& s) const
	{
		std::vector<std::unique_ptr<IElement>> v;
		extractElements<Num>(s,s.size(),v);
		v.push_back(std::make_unique<Empty>("--"));
		v.push_back(std::make_unique<Empty>("--"));

		return v;
	}
};

class BookingTextParser: public ElementParser<BookingTextParser,BookingText, Ref>
{
	friend class ElementParser<BookingTextParser,BookingText, Ref>;
	inline static constexpr const char* Token = " Buchungstext";
	virtual std::vector<std::unique_ptr<IElement>> handle(const std::string& s) const
	{
		std::vector<std::unique_ptr<IElement>> v;
		
		extractElements<Num>(s,s.size(),v);
		v.push_back(std::make_unique<Empty>("--"));
		v.push_back(std::make_unique<Empty>("--"));
		v.push_back(std::make_unique<Empty>("--"));

		return v;
	}
};

class HeaderParser: public ElementParser<HeaderParser, Header>
{
	friend class ElementParser<HeaderParser, Header>;
	inline static constexpr const char* Token = "Buchungstext";
	virtual std::vector<std::unique_ptr<IElement>> handle(const std::string& s) const
	{
		std::vector<std::unique_ptr<IElement>> v;
		
		v.push_back(std::make_unique<Header>(Name::Identifier));
		v.push_back(std::make_unique<Header>(IBAN::Identifier));
		v.push_back(std::make_unique<Header>(BIC::Identifier));
		v.push_back(std::make_unique<Header>(BookingText::Identifier));
		v.push_back(std::make_unique<Header>(Ref::Identifier));
		
		return v;
	}
};
