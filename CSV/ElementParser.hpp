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
				std::cout<<"FOUND "<<T::Pattern<<" "<<String_::Trim(std::string(s.begin()+i,s.begin()+end))<<std::endl;
	  	if (i!=std::string::npos)
			return T::Extract(String_::Trim(std::string(s.begin()+i,s.begin()+end)));
		return T::Make(s);
	}
	template<size_t N>
	static void reg()
	{
		if constexpr (std::tuple_size<Elements>()==N)
			return;
		else
		{
			using Type = std::tuple_element_t<N,Elements>;
			reg<N+1>();
		}
	}
public:
	ElementParser(): token{D::Token} {}
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
	friend class ElementParser<ReceiverParser>;
	inline static constexpr const char* Token = "Empf";
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

		return v;
	}
};

class ClientParser: public ElementParser<ClientParser>
{
	using Elements = std::tuple<Client,BookingText,Ref>;
	static inline constexpr size_t Num = std::tuple_size<Elements>();
	template<size_t N>
	static void extractN(const std::string& s, std::size_t& is)
	{
		if constexpr (0==N)
			return;
		else
		{
			using Type = std::tuple_element_t<N-1,Elements>;
			auto i = s.find(Type::Pattern);
	  		if (i!=std::string::npos)
			{
				extract<Type>(s,is);
			}
			else
				std::cout<<"NOT FOUND "<<Type::Pattern<<" "<<s<<std::endl;
			extractN<N-1>(s,is);
		}
	}
	friend class ElementParser<ClientParser>;
	inline static constexpr const char* Token = "Auftraggeber";
	virtual std::vector<std::unique_ptr<IElement>> handle(const std::string& s) const
	{
		std::vector<std::unique_ptr<IElement>> v;
		auto i2 = s.size();
		extractN<Num>(s,i2);
		auto i = s.size();
		v.push_back(extract<Ref>(s,i));
		//v.push_back(extract<BookingText>(s,i));
		v.push_back(std::make_unique<BIC>("GENODE61DET"));
		v.push_back(std::make_unique<BIC>("GENODE61DET"));
		v.push_back(std::make_unique<BIC>("GENODE61DET"));
		v.push_back(extract<Client>(s,i));
		reg<0>();

		return v;
	}
};

class BookingTextParser: public ElementParser<BookingTextParser>
{
	friend class ElementParser<BookingTextParser>;
	inline static constexpr const char* Token = " Buchungstext";
	virtual std::vector<std::unique_ptr<IElement>> handle(const std::string& s) const
	{
		std::vector<std::unique_ptr<IElement>> v;
		
		auto i = s.size();
		v.push_back(extract<Ref>(s,i));
		v.push_back(extract<BookingText>(s,i));
		v.push_back(std::make_unique<BIC>("GENODE61DET"));
		v.push_back(std::make_unique<BIC>("GENODE61DET"));
		v.push_back(std::make_unique<BIC>("GENODE61DET"));
		reg<0>();

		return v;
	}
};

class HeaderParser: public ElementParser<HeaderParser>
{
	friend class ElementParser<HeaderParser>;
	inline static constexpr const char* Token = "Buchungstext";
	virtual std::vector<std::unique_ptr<IElement>> handle(const std::string& s) const
	{
		std::vector<std::unique_ptr<IElement>> v;
		
		v.push_back(std::make_unique<Header>(s));
		v.push_back(std::make_unique<BIC>("GENODE61DET"));
		v.push_back(std::make_unique<BIC>("GENODE61DET"));
		v.push_back(std::make_unique<BIC>("GENODE61DET"));
		v.push_back(std::make_unique<BIC>("GENODE61DET"));
		
		return v;
	}
};
