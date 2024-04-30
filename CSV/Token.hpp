#include "TokenLiterals.hpp"
#include "../String/String_.hpp"
#include "../Logger/Logger.hpp"
#include "../Common/ArrayHelper.hpp"
#include "../Quantity/Quantity.hpp"
#include "../To/To.hpp"
#include <string.h>
#include <map>
#include <regex>
#include <memory>
#include <array>
#include <chrono>
#include <ctime>

#pragma once
class IBAN;
class BIC;
class Entry;
template<typename> class Value;
class Date;
//class ;
//class ;
class IToken
{
public:
	virtual const std::string_view Data() const  = 0;	
	virtual const std::regex Pattern() const  = 0;	
	virtual bool Match(const std::string&) const  = 0;	
	virtual std::vector<std::unique_ptr<IElement>> operator()(const std::string&) const  = 0;	
	virtual std::unique_ptr<IElement> Create(const std::string&) const  = 0;	
	virtual std::unique_ptr<IToken> Clone() const  = 0;	
	constexpr bool operator==(const IToken& e) const{ return Data() == e.Data(); };
private:
	friend std::ostream& operator<<(std::ostream& out, const IToken& e) { return out<<e.Data();}
};
//--------------------------------Token------------------------------------------------

template<typename, typename> class Token;

template<typename D, typename T = int>
class TokenBase: public IToken
{
	using Derived = D;
	using Type = T;
	inline static constexpr Literal TypeId{TokenIdentifier::TypeIdentifier};
public:
	inline static const std::string Identifier = std::string(T::Identifier) + TokenIdentifier::TypeIdentifier;
 	TokenBase(const std::string& s = ""): exclude{s} { };
	const std::string_view Data() const  {	return Derived::Pattern; };	
	bool Match(const std::string& s) const  {	return exclude == "" ? std::regex_match(s,pattern) : std::regex_match(s,pattern) && !std::regex_match(s,std::regex(exclude)); };	
	virtual std::unique_ptr<IElement> Create(const std::string& s) const  	{	return std::make_unique<Type>(s); 	};	
	virtual const std::regex Pattern() const { return pattern; };	
	virtual std::vector<std::unique_ptr<IElement>> operator()(const std::string& s) const  
	{
		std::vector<std::unique_ptr<IElement>> result;
		std::smatch match;

		std::regex rgx(";");
		std::sregex_token_iterator iter(s.begin(),s.end(), rgx, -1);
		std::sregex_token_iterator end;
		for ( ; iter != end; ++iter)
		{
			std::string si(*iter);
			if (Match(si))
				result.push_back(std::make_unique<Type>(si));
		}
		return result;
	};	
	virtual std::unique_ptr<IToken> Clone() const  { return std::make_unique<Derived>();};	
	static std::unique_ptr<IToken> Make(const std::string& s) { return std::make_unique<Derived>(); };	

	bool operator==(const TokenBase& e) const{ return Data() == e.Data(); };
protected:
	using Base = Token<D,T>;
private:
	std::regex pattern = std::regex( Derived::Pattern );
	std::string exclude;
};

template<typename D, typename T = int>
class Token: public TokenBase<D,T>
{
protected:
 	Token(const std::string& s = ""): TokenBase<D,T>{s} { };
};

template<typename D, typename T>
struct Token<D,Index<T>>: public TokenBase<D,Index<T>> 
{
 	virtual std::unique_ptr<IElement> Create(const std::string& s) const  {  return std::make_unique<T>(s);    };
};

template<typename D>
struct Token<D,Index<Entry>>: public TokenBase<D,Index<Entry>> { inline static const std::string Identifier = D::Pattern + std::string("IndexToken"); };

struct IBANToken: public Token<IBANToken, IBAN>
{	
	IBANToken(const std::string& e = ""): Token<IBANToken, IBAN>(e) {}	
	inline static constexpr const char* Pattern = "^DE\\d{20}$";
};

struct DateToken: public Token<DateToken, Date>				{	inline static constexpr const char* Pattern = "(0?[1-9]|[1-2][0-9]|3[0-1]).(0?[1-9]|1[0-2]).(\\d{4})";};
struct EntryToken: public Token<EntryToken, Entry>			{	inline static constexpr const char* Pattern = "(\\d|\\s|\\w|[.,:/])+";};
struct BICToken: public Token<BICToken, BIC>				{	inline static constexpr const char* Pattern = "([a-zA-Z]{4}[a-zA-Z]{2}[a-zA-Z0-9]{2}([a-zA-Z0-9]{3})?)"; };
struct SumToken: public Token<SumToken, Quantity<Sum>>		{	inline static constexpr const char* Pattern = "((^[-+]?((\\d+[.,]\\d{2})|([1-9]\\d+))€$)|(^[-+]?((\\d+[.,]\\d{2}))$))";};
struct VolumeToken: public Token<VolumeToken, Quantity<Volume>>	{	inline static constexpr const char* Pattern = "(((\\d+[.,]\\d+))m\\^[\\(]?3[\\)]?)";};
struct WorkToken: public Token<WorkToken, Quantity<Energy, KiloHour>>	{	inline static constexpr const char* Pattern = "(((\\d+[.,]\\d+))kWs$)";};
struct ValueToken: public Token<ValueToken, Value<int>>		{	inline static constexpr const char* Pattern = ""; };
struct WordToken: public Token<WordToken, Entry>			{	inline static constexpr const char* Pattern = "[a-zA-z]+";};
struct IDToken: public Token<IDToken, ID<int>>				{	inline static constexpr const char* Pattern = "^[0-9]+$";};
struct EmptyToken: public Token<EmptyToken, Empty>			{	inline static constexpr const char* Pattern = "^(\\s*$)";};
struct QuantityToken: public Token<QuantityToken, Quantity<Scalar>>	{	inline static constexpr const char* Pattern = ""; };

struct DateIndexToken: public Token<DateIndexToken, Index<Date>>	{	inline static constexpr const char* Pattern = "Buchungstag";};
struct NameIndexToken: public Token<NameIndexToken, Index<Name>>	{	inline static constexpr const char* Pattern = "(\\w)*[Nn]ame(\\w)*";};
struct IBANIndexToken: public Token<IBANIndexToken, Index<IBAN>>	{	inline static constexpr const char* Pattern = "IBAN Zahlungsbeteiligter";};
struct BICIndexToken: public Token<BICIndexToken, Index<BIC>>	{	inline static constexpr const char* Pattern = "BIC \\(SWIFT-Code\\) Zahlungsbeteiligter"; };
struct SumIndexToken: public Token<SumIndexToken, Index<Quantity<Sum>>>	{	inline static constexpr const char* Pattern = "Betrag";};
struct UseIndexToken: public Token<UseIndexToken, Index<Entry>>	{	inline static constexpr const char* Pattern = "Verwendungszweck";};

struct WasteIndexToken: public Token<WasteIndexToken, Index<Quantity<Scalar>>>	
{	
	inline static constexpr const char* Pattern = "Waste"; 
	inline static constexpr const char* Identifier = "WasteIndexToken"; 
};
struct HeatingIndexToken: public Token<HeatingIndexToken, Index<Quantity<Area>>>	
{	
	inline static constexpr const char* Pattern = "Heating"; 
	inline static constexpr const char* Identifier = "HeatingIndexToken"; 
};
struct StageIndexToken: public Token<StageIndexToken, Index<Entry>>	
{	
	inline static constexpr const char* Pattern = "Stage"; 
	inline static constexpr const char* Identifier = "StageIndexToken"; 
};
struct CleaningIndexToken: public Token<CleaningIndexToken, Index<Quantity<Scalar>>>	
{	
	inline static constexpr const char* Pattern = "Cleaning"; 
	inline static constexpr const char* Identifier = "CleaningIndexToken"; 
};
struct SewageIndexToken: public Token<SewageIndexToken, Index<Quantity<Scalar>>>	
{	
	inline static constexpr const char* Pattern = "Sewage"; 
	inline static constexpr const char* Identifier = "SewageIndexToken"; 
};
struct PropertyTaxIndexToken: public Token<PropertyTaxIndexToken, Index<Quantity<Area>>>	
{	
	inline static constexpr const char* Pattern = "PropertyTax"; 
	inline static constexpr const char* Identifier = "PropertyTaxIndexToken"; 
};
struct InsuranceIndexToken: public Token<InsuranceIndexToken, Index<Quantity<Scalar>>>	
{	
	inline static constexpr const char* Pattern = "Insurance"; 
	inline static constexpr const char* Identifier = "InsuranceIndexToken"; 
};
struct RentIndexToken: public Token<RentIndexToken, Index<Quantity<Sum>>>	
{	
	inline static constexpr const char* Pattern = "Rent"; 
	inline static constexpr const char* Identifier = "RentIndexToken"; 
};
struct ExtraCostIndexToken: public Token<ExtraCostIndexToken, Index<Quantity<Sum>>>	
{	
	inline static constexpr const char* Pattern = "ExtraCosts"; 
	inline static constexpr const char* Identifier = "ExtraCostsIndexToken"; 
};
struct HeatExtraCostIndexToken: public Token<HeatExtraCostIndexToken, Index<Quantity<Sum>>>	
{	
	inline static constexpr const char* Pattern = "HeatExtraCosts"; 
	inline static constexpr const char* Identifier = "HeatExtraCostsIndexToken"; 
};

struct GarageRentIndexToken: public Token<GarageRentIndexToken, Index<Quantity<Sum>>>	
{	
	inline static constexpr const char* Pattern = "GarageRent"; 
	inline static constexpr const char* Identifier = "GarageRentIndexToken"; 
};

struct KeyValueToken: public Token<KeyValueToken, Entry>	{ 	inline static constexpr const char* Pattern = "([a-zA-z])+\\s?:\\s?([\\w\\d]+)";};
