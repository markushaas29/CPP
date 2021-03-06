#include <algorithm>
#include <functional>
#include <iostream>
#include <fstream>
#include <chrono>
#include <ctime>
#include <iterator>
#include <vector>
#include <cstdlib>
#include <unordered_map>
#include <boost/mpl/vector.hpp>
#include <boost/filesystem.hpp>
#include <boost/mpl/for_each.hpp>
#include <boost/mpl/placeholders.hpp>
#include <filesystem>
#include "../Logger/Logger.hpp"
#include "../CSV/CSV.hpp"
#include "../Quantity/Quantity.h"
#include "../Typelist/Typelist.h"
#include "../Visitor/Visitor.hpp"
#include "../String/String_.hpp"

#ifndef PARSETYPES_HPP
#define PARSETYPES_HPP

namespace fs = std::filesystem;

namespace FS
{

	class Code
	{
		
		std::string line;
		uint lineNumber;
	public:
		using ParseType = Code;
		using ParseCont = std::vector<ParseType>;
		
		Code(uint lineNo, std::string l): lineNumber(lineNo), line(l){};
		
		const uint LineNumber() const { return lineNumber; }
		const std::string& Line() const { return line; }
		
		static ParseCont Parse(std::vector<std::string> content)
		{
			uint ctr = 0;
			auto result = ParseCont();
			for(auto l : content)
				result.push_back(ParseType(++ctr, l));
			
			return result;
		}
		
	};
	
	std::ostream& operator<<(std::ostream& out, const Code& c)
	{
		return out<<c.LineNumber()<<"\t"<<c.Line();
	}
	
	//-----------------------------------------------------------------------------------------------TranferTypes-----------------------------------------------------------------------
	
	template<typename Direction>
	struct Transfer
	{
		using Type = Transfer<Direction>;
		using ValueType = Direction;
		inline static const std::string Id = Direction::Id;
	};
	
	struct In
	{
		using Type = In;
		inline static const std::string Id = "In"; 
	};
	
	struct Out
	{
		using Type = Out;
		inline static const std::string Id = "Out"; 
	};
	
	//-----------------------------------------------------------------------------------------------AccountTransfer-----------------------------------------------------------------------
	
	template<typename Account, typename Direction>
	class AccountTransfer
	{
		using Type = AccountTransfer<Account,Direction> ;
		
		Key owner;
		Entry transaction;
		Date date;
		IBAN iban;
		BIC bic;
		Quantity<Sum> value;

	protected:
		using CSVSeparator = T::char_<';'> ;
	public:
		using AccountType = Account ;
		using DirectionType = Direction ;
		using KeyType = Key;
		using QunatityType = Quantity<Sum>;
		
		AccountTransfer(std::string k, std::string c, double v, std::string d, std::string i = "IBAN", std::string b = "BIC") : owner(k), transaction(c), date(d), value(v), iban(i), bic(b) { };
		
		const Key& GetOwner() const { return owner; }
		const Entry& GetTransaction() const { return transaction; }
		const Date& GetDate() const { return date; }
		const IBAN& GetIBAN() const { return iban; }
		const BIC& GetBIC() const { return bic; }
		const Quantity<Sum>& GetQuantity() const { return value; }
		const Direction& GetDirection() const { return Direction::Id; }		
	};
	
	//-----------------------------------------------------------------------------------------------TranferEndpoint-----------------------------------------------------------------------
	
	template<typename Account, typename Direction, template<typename> class Cont = std::vector>
	class AccountEndpoint
	{
		using Type = AccountEndpoint<Account,Direction> ;
		using TransferType = AccountTransfer<Account,Direction> ;
		using ContType = Cont<TransferType> ;
		
		Key owner;
		IBAN iban;
		BIC bic;
		Quantity<Sum> total;
		ContType transfers;

	protected:
		using CSVSeparator = T::char_<';'> ;
	public:
		using KeyType = Key;
		using QunatityType = Quantity<Sum>;
		
		AccountEndpoint(std::string ownerKey, std::string i = "IBAN", std::string b = "BIC") : owner(ownerKey), iban(i), bic(b) { };
		AccountEndpoint(const TransferType& t) : owner(t.GetOwner()), iban(t.GetIBAN()), bic(t.GetBIC()), total(t.GetQuantity()) { };
		AccountEndpoint():owner("ownerKey"), iban("i"), bic("b"), total(0) { };
		
		const Key& GetOwner() const { return owner; }
		const ContType& GetTransfers() const { return transfers; }
		const IBAN& GetIBAN() const { return iban; }
		const BIC& GetBIC() const { return bic; }
		const Quantity<Sum>& GetTotal() const { return total; }
		const Direction& GetDirection() const { return Direction::Id; }		
		
		void Add(TransferType t)
		{
			this->transfers.push_back(t);
			this->total = this->total + t.GetQuantity();
		}
		
		void Display(std::ostream& out)
		{
			out<<"Owner: "<<owner<<"\tIBAN: "<<iban<<"\tBIC: "<<bic<<std::endl;
			for(auto p : this->transfers)
			{
				out<<"\tDate: "<<p.GetDate()<<"\tSum: "<<p.GetQuantity()<<std::endl;
			}

			out<<std::endl;
		}
	};
	
	//-----------------------------------------------------------------------------------------------TranferContainer-----------------------------------------------------------------------
		
	template<typename T, template<typename, typename> class TCont = std::map, template<typename> class Cont = std::vector>
	class TransferContainer
	{
	public:
		using KeyType = typename T::KeyType;
		using AccountEndpointType = AccountEndpoint<typename T::AccountType, typename T::DirectionType>;
	private:
		Cont<KeyType> keys;
		TCont<KeyType, AccountEndpointType> transfers;
	public:
		void Insert(KeyType k, T t)
		{
			if(!this->Contains(k))
			{
				this->keys.push_back(k);
				this->transfers.insert(std::pair<KeyType, AccountEndpointType>(k,AccountEndpointType(t)));
				return;
			}
			
			this->transfers[k].Add(t);
		}
		
		bool Contains(KeyType k){ return this->transfers.find(k) != this->transfers.end(); }
		const Cont<KeyType>& Keys() { return keys; }
		const AccountEndpointType& operator[](KeyType k){ return this->transfers[k]; }
		
		void Display(std::ostream& out)
		{
			for(auto p : this->transfers)
			{
				out<<p.first<<":"<<std::endl;
				
				p.second.Display(out);
				
				out<<"\nTotal: "<<p.second.GetTotal()<<std::endl;
				
				out<<std::endl;
			}
		}
	};
	
	
	template<typename Derived>
	class Account
	{
		using Type = Account<Derived> ;
	protected:		
// 		inline static T::Is_<typename Derived::IsOutTransferSign> IsOutTransfer;
		using CSVSeparator = T::char_<';'> ;
	public:
		Key owner;
		IBAN iban;
		BIC bic;
		using InTransfer = AccountTransfer<Derived,Transfer<In>>;
		using OutTransfer = AccountTransfer<Derived, Transfer<Out>>;
		using KeyType = Key ;
		using ParseContIn = TransferContainer<AccountTransfer<Derived,Transfer<In>>>;
		using ParseContOut = TransferContainer<AccountTransfer<Derived,Transfer<Out>>>;
		using QunatityType = Quantity<Sum>;
		
		Account(std::string k, std::string c, double v, std::string d, std::string i = "IBAN", std::string b = "BIC") : owner(k), iban(i), bic(b) { };
		
		static constexpr unsigned int Indices[4] = {Derived::OwnerIdx, Derived::DateIdx, Derived::TranactionIdx, Derived::QuantityIdx};
		static const unsigned int MaxIdx = *std::max_element(Indices,Indices+4);
		
		static void Parse(std::vector<std::string> content)
		{
			if(content.size() > 0)
			{
				uint ctr = 0;
							
// 				Logger::Log(content.cbegin(), content.cend());
				
				auto header = RemoveHeader(content);
				auto trailer = RemoveTrailer(content);
	
				for(auto line : content)
				{
					auto values = String_::Split<CSVSeparator>(line);
					
					if (values.size() < MaxIdx)
						continue;
										
					Derived::ProcessValues(values);
					
				}
			}

			return;
		}
		
		static std::string GetNumericValue(std::string s)
		{
			std::string result;
			for (unsigned int i = 0; i < s.size(); ++i)
			{
				if(std::isdigit(s.at(i)))
					result += s.at(i);
			}
			
			if(result.size() > 2)
				result.insert(result.size()-2, ".");
			
			return result;
		}
				
		template<typename Cont>
		static void AttachTo(Cont& cont)
		{
			cont.insert(std::make_pair(Derived::Filename,  &Type::Parse));
		}	
		
		static std::vector<std::string> RemoveHeader(std::vector<std::string>&  cont)
		{
			std::vector<std::string> header;
			std::copy(cont.begin(), cont.begin() + Derived::HeaderLength, std::back_inserter(header));
			cont.erase(cont.begin(), cont.begin() + Derived::HeaderLength);
			return header;
		}	
		
		static std::vector<std::string> RemoveTrailer(std::vector<std::string>&  cont)
		{
			std::vector<std::string> header;
			std::copy(cont.end() - Derived::TrailerLength, cont.end(), std::back_inserter(header));
			cont.erase(cont.end() - Derived::TrailerLength, cont.end());
			return header;
		}	
		
	protected:
		static void InsertInContainer(std::string key, std::string transaction, double sum, std::string date, std::string iban, std::string bic, char transferSign)
		{
			if(Derived::IsOutTransfer(transferSign))
				Derived::OutCont.Insert(key, OutTransfer(key,transaction,sum, date, iban, bic));
			else
				Derived::InCont.Insert(key, InTransfer(key,transaction,sum, date, iban, bic));
			
		}
	};
	
	class JSONParser
	{
		using Type = std::string;
		using ResultType = std::map<Type, Type>;
		using JSON_Separator = T::char_<':'> ;
		using BlankSeparator = T::char_<' '> ;
		ResultType resultMap;

	public:	
		template<typename Iterator>
		ResultType Parse(Type key, Iterator first, Iterator end)
		{
			if(first == end)
				return this->resultMap;
			
			auto vals = String_::Split<BlankSeparator>(*first);
			auto value = Type();
					
			for(int i = 0; i < vals.size()-1; ++i)
				value += vals.at(i);
			
			this->resultMap[key] = value;
			this->resultMap.insert({key, value});			
			key = vals.at(vals.size()-1);

			return Parse(key, ++first,end);
		}
		
		ResultType Parse(Type input)
		{
			auto vals = String_::Split<JSON_Separator>(input);
				
			return Parse(*vals.begin(), ++vals.begin(),vals.end());
		}
	};
	
	//-----------------------------------------------------------------------------------------------Tranfers-----------------------------------------------------------------------
	template<unsigned int N = 0>
	struct Comdirect: public Account<Comdirect<N>>
	{
		enum{ Num = N };
		using InType = AccountTransfer<Comdirect,Transfer<In>>;
		using OutType = AccountTransfer<Comdirect,Transfer<Out>>;
		using IsOutTransferSign = T::char_<'-'>;
		using Base = Account<Comdirect>;
		
		inline static T::Is_<IsOutTransferSign> IsOutTransfer;
		inline static const std::string Name = "Comdirect";
		inline static const std::string Filename = "Umsaetze_1026947527.csv";
		inline static constexpr unsigned int OwnerIdx = 3;
		inline static constexpr unsigned int TranactionIdx = 3;
		inline static constexpr unsigned int DateIdx = 0;
		inline static constexpr unsigned int QuantityIdx = 4;
		inline static constexpr unsigned int HeaderLength = 5;
		inline static constexpr unsigned int TrailerLength = 23;
		
		inline static Base::ParseContIn InCont = typename Base::ParseContIn();
		inline static Base::ParseContOut OutCont = typename Base::ParseContOut();
		Comdirect(std::string k, std::string c, double v, std::string d, std::string i = "IBAN", std::string b = "BIC") : Base(k,c,v, d, i, b) {};
		
		static void Display(std::ostream& os)
		{
			os<<"IN"<<std::endl;
			InCont.Display(os);
			
			os<<"OUT"<<std::endl;
			OutCont.Display(os);
		}
		
		using TextSeparator = T::char_<' '> ;
		
		template<typename T>
		static std::string Extract(std::string s)
		{
			JSONParser json;
			auto vals = json.Parse(s);

			auto it = std::find_if (vals.begin(),vals.end(), [](std::pair<std::string, std::string> const& item) { return String_::Contains(item.first,T::Identifier); } );
			
			if(it == vals.end())
				return "";
						
			return (it)->second;
		}
		
		static std::string ExtractKey(std::string s)
		{
			JSONParser json;
			if(s == "\"Buchungstext\"")
				return "";
			
			auto vals = json.Parse(s);
			
			return vals.begin()->second;
		}
		
		
		
		static void ProcessValues(std::vector<std::string> values)
		{
			auto keyLine = values.at(OwnerIdx);
			if(keyLine != "")
			{
				auto key = ExtractKey(keyLine);
				auto date = values.at(DateIdx);
				auto transaction = values.at(TranactionIdx);
				
				auto n = Base::GetNumericValue(values.at(QuantityIdx));
				auto sum = n != "" ? std::stod(n) : 0.0 ;
			
				auto iban =  Extract<IBAN>(transaction);
				auto bic = Extract<BIC>(transaction);
								
				Base::InsertInContainer(key,transaction,sum, date, iban, bic, *(values.at(QuantityIdx).begin()+1));
			}
				
		}
	};
	
	template<unsigned int N = 0>
	struct Raiba: public Account<Raiba<N>>
	{
		enum{ Num = N };
		using InType = AccountTransfer<Raiba,Transfer<In>>;
		using OutType = AccountTransfer<Raiba,Transfer<Out>>;
		using IsOutTransferSign = T::char_<'S'>;
		using Base = Account<Raiba>;
		
		
		inline static T::Is_<IsOutTransferSign> IsOutTransfer;
		inline static const std::string Name = "Raiba";
		inline static const std::string Filename = "Umsaetze_DE19660623660009232702.csv";
		inline static constexpr unsigned int OwnerIdx = 4;
		inline static constexpr unsigned int TranactionIdx = 9;
		inline static constexpr unsigned int IBANIdx = 6;
		inline static constexpr unsigned int BICIdx = 7;
		inline static constexpr unsigned int DateIdx = 0;
		inline static constexpr unsigned int QuantityIdx = 12;
		inline static constexpr unsigned int HeaderLength = 16;
		inline static constexpr unsigned int TrailerLength = 3;
		
		inline static Base::ParseContIn InCont = typename Base::ParseContIn();
		inline static Base::ParseContOut OutCont = typename Base::ParseContOut();
		Raiba(std::string k, std::string c, double v, std::string d, std::string i = "IBAN", std::string b = "BIC") : Base(k,c,v, d, i, b) {};
		
		static void Display(std::ostream& os)
		{
			os<<"IN"<<std::endl;
			InCont.Display(os);
			
			os<<"OUT"<<std::endl;
			OutCont.Display(os);
		}
		
		template<typename T>
		static std::string Extract(std::string s)
		{
			JSONParser json;
			auto vals = json.Parse(s);

			auto it = std::find_if (vals.begin(),vals.end(), [](std::pair<std::string, std::string> const& item) { return String_::Contains(item.first,T::Identifier); } );
			
			if(it == vals.end())
				return "";
						
			return (it)->second;
		}
		static std::string ExtractKey(std::string s)
		{
			return s;
		}
		
		static void ProcessValues(std::vector<std::string> values)
		{
			auto keyLine = values.at(OwnerIdx);
			if(keyLine != "")
			{
				auto key = ExtractKey(keyLine);
				auto date = values.at(DateIdx);
				auto transaction = values.at(TranactionIdx);
				
				auto val = values.at(QuantityIdx);
				std::string::iterator end_pos = std::remove(val.begin(), val.end(), ' ');
				val.erase(end_pos, val.end());

				auto sum = std::stod(*(values.end()-2));
			
				auto iban =  values.at(IBANIdx);
				auto bic =  values.at(BICIdx);
				
				std::string sign = *(values.end()-1);
				Base::InsertInContainer(key,transaction,sum, date, iban, bic,sign[0]);
			}
		}				
	};
	
	template<unsigned int N = 0>
	struct Custom: public Account<Custom<N>>
	{
		enum{ Num = N };
		using InType = AccountTransfer<Custom,Transfer<In>>;
		using OutType = AccountTransfer<Custom,Transfer<Out>>;
		using IsOutTransferSign = T::char_<'-'>;
		using Base = Account<Custom>;
		
		inline static T::Is_<IsOutTransferSign> IsOutTransfer;
		inline static const std::string Name = "Custom";
		inline static const std::string Filename = "RaibaKonten2021_1.csv";
		inline static constexpr unsigned int OwnerIdx = 1;
		inline static constexpr unsigned int TranactionIdx = 2;
		inline static constexpr unsigned int DateIdx = 0;
		inline static constexpr unsigned int QuantityIdx = 3;
		inline static constexpr unsigned int HeaderLength = 15;
		inline static constexpr unsigned int TrailerLength = 4;
		
		inline static Base::ParseContIn InCont = typename Base::ParseContIn();
		inline static Base::ParseContOut OutCont = typename Base::ParseContOut();
		Custom(std::string k, std::string c, double v, std::string d, std::string i = "IBAN", std::string b = "BIC") : Base(k,c,v, d, i, b) {};		
		
		static void Display(std::ostream& os)
		{
			InCont.Display(os);
		}
		
		template<typename T>
		static std::string Extract(std::string s)
		{
			return s;
		}
		
		static std::string ExtractKey(std::string s)
		{
			return s;
		}
		
		static void ProcessValues(std::vector<std::string> values)
		{
			auto keyLine = values.at(OwnerIdx);
			if(keyLine != "")
			{
				auto key = ExtractKey(keyLine);
				auto date = values.at(DateIdx);
				auto transaction = values.at(TranactionIdx);
				
				auto n = Base::GetNumericValue(values.at(QuantityIdx));
				auto sum = n != "" ? std::stod(n) : 0.0 ;
			
				auto iban =  Extract<IBAN>(transaction);
				auto bic = Extract<BIC>(transaction);

				InCont.Insert(key, typename Base::InTransfer(key,transaction,sum, date, iban, bic));
				OutCont.Insert(key, typename Base::OutTransfer(key,transaction,sum, date, iban, bic));
				
				Base::InsertInContainer(key,transaction,sum, date, iban, bic, *(values.at(QuantityIdx).begin()+1));
			}
		}				
				

	};
	template<typename T, typename D>
	std::ostream& operator<<(std::ostream& out, const AccountTransfer<T,D>& s)
	{
		out<<std::setw(30)<<std::left<<s.GetOwner()<<std::setw(60)<<s.GetTransaction()<<std::setw(20)<<std::right<<s.GetDate()<<std::setw(10)<<std::setprecision(2)<<std::fixed<<s.GetQuantity()<<"\n";
		return out<<std::setw(30)<<std::left<<s.GetIBAN()<<std::setw(60)<<s.GetBIC()<<std::setw(20);		
	}
}


#endif
