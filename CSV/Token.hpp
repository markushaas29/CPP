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

template<typename D, typename T = int>
class Token: public IToken
{
	using Derived = D;
	using Type = T;
public:
	inline static const std::string Identifier = std::string(T::Identifier) + "Token";
 	Token(const std::string& s = ""): exclude{s} { };
	const std::string_view Data() const  {	return Derived::Pattern; };	
	bool Match(const std::string& s) const  {	return exclude == "" ? std::regex_match(s,pattern) : std::regex_match(s,pattern) && !std::regex_match(s,std::regex(exclude)); };	
	virtual std::unique_ptr<IElement> Create(const std::string& s) const  { return std::make_unique<Type>(s); };	
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

	bool operator==(const Token& e) const{ return Data() == e.Data(); };
protected:
	using Base = Token<D,T>;
private:
	std::regex pattern = std::regex( Derived::Pattern );
	std::string exclude;
};

struct IBANToken: public Token<IBANToken, IBAN>
{	
	IBANToken(const std::string& e = ""): Base(e) {}	
	inline static constexpr const char* Pattern = "^DE\\d{20}$";
};
struct DateToken: public Token<DateToken, Date>				{	inline static constexpr const char* Pattern = "(0?[1-9]|[1-2][0-9]|3[0-1]).(0?[1-9]|1[0-2]).(\\d{4})";};
struct EntryToken: public Token<EntryToken, Entry>			{	inline static constexpr const char* Pattern = "(\\d|\\s|\\w|[.,:/])+";};
struct BICToken: public Token<BICToken, BIC>				{	inline static constexpr const char* Pattern = "([a-zA-Z]{4}[a-zA-Z]{2}[a-zA-Z0-9]{2}([a-zA-Z0-9]{3})?)"; };
struct SumToken: public Token<SumToken, Quantity<Sum>>		{	inline static constexpr const char* Pattern = "((^[-+]?((\\d+[.,]\\d{2})|([1-9]\\d+))€$)|(^[-+]?((\\d+[.,]\\d{2}))$))";};
struct VolumeToken: public Token<VolumeToken, Quantity<Volume>>	{	inline static constexpr const char* Pattern = "(((\\d+[.,]\\d+))m\\^[\\(]?3[\\)]?)";};
struct EnergyToken: public Token<EnergyToken, Quantity<Energy, KiloHour>>	{	inline static constexpr const char* Pattern = "(((\\d+[.,]\\d+))kWs$)";};
struct ValueToken: public Token<ValueToken, Value<int>>		{	inline static constexpr const char* Pattern = ""; };
struct WordToken: public Token<WordToken, Entry>			{	inline static constexpr const char* Pattern = "[a-zA-z]+";};
struct IDToken: public Token<IDToken, ID<int>>				{	inline static constexpr const char* Pattern = "^[0-9]+$";};
struct EmptyToken: public Token<EmptyToken, Empty>			{	inline static constexpr const char* Pattern = "^(\\s*$)";};

struct DateIndexToken: public Token<DateIndexToken, Date>	{	inline static constexpr const char* Pattern = "Buchungstag";};
struct NameIndexToken: public Token<NameIndexToken, Entry>	{	inline static constexpr const char* Pattern = "(\\w)*[Nn]ame(\\w)*";};
struct IBANIndexToken: public Token<IBANIndexToken, Entry>	{	inline static constexpr const char* Pattern = "IBAN Zahlungsbeteiligter";};
struct BICIndexToken: public Token<BICIndexToken, Entry>	{	inline static constexpr const char* Pattern = "BIC \\(SWIFT-Code\\) Zahlungsbeteiligter"; };
struct SumIndexToken: public Token<SumIndexToken, Entry>	{	inline static constexpr const char* Pattern = "Betrag";};
struct UseIndexToken: public Token<UseIndexToken, Entry>	{	inline static constexpr const char* Pattern = "Verwendungszweck";};
struct QuantityToken: public Token<QuantityToken, Quantity<Scalar>>	{	inline static constexpr const char* Pattern = ""; };

struct WasteIndexToken: public Token<WasteIndexToken, Entry>	{	inline static constexpr const char* Pattern = "Waste"; };
struct HeatingIndexToken: public Token<HeatingIndexToken, Entry>	{	inline static constexpr const char* Pattern = "Heating"; };
struct StageIndexToken: public Token<StageIndexToken, Entry>	{	inline static constexpr const char* Pattern = "Stage"; };
struct CleaningIndexToken: public Token<CleaningIndexToken, Entry>	{	inline static constexpr const char* Pattern = "Cleaning"; };
struct SewageIndexToken: public Token<SewageIndexToken, Entry>	{	inline static constexpr const char* Pattern = "Sewage"; };
struct PropertyTaxIndexToken: public Token<PropertyTaxIndexToken, Entry>	{	inline static constexpr const char* Pattern = "PropertyTax"; };
struct InsuranceIndexToken: public Token<InsuranceIndexToken, Entry>	{	inline static constexpr const char* Pattern = "Insurance"; };

struct KeyValueToken: public Token<KeyValueToken, Entry>	{ 	inline static constexpr const char* Pattern = "([a-zA-z])+\\s?:\\s?([\\w\\d]+)";};
