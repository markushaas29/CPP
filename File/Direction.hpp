#include <algorithm>
#include <functional>
#include <iostream>
#include <memory>
#include <vector>
#include "TransferContainer.hpp"
#include "../Logger/Logger.hpp"
#include "../CSV/CSV.hpp"
#include "../Wrapper/Wrapper.hpp"
#include "../Quantity/Quantity.h"
#include "../Unit/SIPrefix.hpp"
#include "../Typelist/Typelist.h"
#include "../Visitor/Visitor.hpp"
#include "../String/String_.hpp"
#include "../Common/DateTimes.hpp"

#ifndef DIRECTION_HPP
#define DIRECTION_HPP

namespace fs = std::filesystem;

namespace Bank
{
	template<typename T>
	bool Is(char c)
	{
		for(auto it = T::signs->cbegin(); it != T::signs->cend(); ++it)
			if(*it == c)
				return true;
				
		return false;
	}
	
	class DirectionType
	{
	public:
		using VectorType = std::vector<char>;
		using PtrType = std::unique_ptr<VectorType>;
		virtual const std::string& Sign() = 0;	
		virtual const int Value() = 0;	
		virtual const std::string& TypeIdentifier() = 0;	
	};
	
	class TransferIn: public DirectionType
	{
		template<typename T>
		friend bool Is(char c); 
		
		inline static PtrType signs = PtrType(new VectorType{'+','H','1','2','3','4','5','6','7','8','9'});
	public:
		using Type = TransferIn;
		using SignType = T::char_<'+'>;
		inline static const std::string TypeId = "TransferIn"; 
		inline static constexpr int Id = 1; 
		inline static constexpr char SignValue = SignType::Value; 
		virtual const std::string& Sign() { return SignType::String; };		
		virtual const int Value() { return Id;};	
		virtual const std::string& TypeIdentifier() { return TypeId; };	
	};
	
	class TransferOut: public DirectionType
	{
		template<typename T>
		friend bool Is(char c); 
		
		inline static PtrType signs = PtrType(new VectorType{'-','S'});
	public:
		using Type = TransferOut;
		using SignType = T::char_<'-'>;
		inline static const std::string TypeId = "TransferOut"; 
		inline static constexpr int Id = -1; 
		inline static constexpr char SignValue = SignType::Value; 
		virtual const std::string& Sign() { return SignType::String; };		
		virtual const int Value() { return Id;};	
		virtual const std::string& TypeIdentifier() { return TypeId;};	
	};
	
	class UnknownDirection: public DirectionType
	{
	public:
		using Type = UnknownDirection;
		using SignType = T::char_<'?'>;
		inline static const std::string TypeId = "UnknownDirection"; 
		inline static constexpr int Id = 0; 
		inline static constexpr char SignValue = SignType::Value; 
		virtual const std::string& Sign() { return SignType::String; };		
		virtual const int Value() { return Id;};	
		virtual const std::string& TypeIdentifier() { return TypeId; };	
	};
	
	template<typename TIn=TransferIn, typename TOut=TransferOut, typename TUnknown=UnknownDirection>
	class DirectionBase: public Element
	{
	public:
		DirectionBase(std::string s): Element(s), tranferType{Create(*(s.cbegin()))},value{UnknownDirection::Id}, id{UnknownDirection::Id}, typeId{UnknownDirection::TypeId} { };
		using Type = DirectionBase;
		using QuantityType = Quantity<Scalar,SIPrefix<0>>;
		using PtrType = std::shared_ptr<DirectionType>;
		
		inline static constexpr const char* Identifier = "Direction";
		
		DirectionBase* DoCreate(){return this;};
		DirectionBase(const DirectionBase&) = default;
		const auto& Value() const {	return this->tranferType->Value(); }
		const auto& Id() const  {	return this->tranferType->Value(); }
		const auto& TypeId() const  {	return this->tranferType->TypeIdentifier(); }	
		const auto QuantityValue() const  { return Quantity<Scalar,SIPrefix<0>,int>{this->tranferType->Value()}; }	
	
		static PtrType Create(const char c)
		{
			if(Is<TransferIn>(c))
				return std::make_unique<TransferIn>();
			if(Is<TransferOut>(c))
				return std::make_unique<TransferOut>();
			return std::make_unique<UnknownDirection>();
		}
				
		std::ostream& Display(std::ostream& os) const {	return os<<tranferType->Sign();	}
	protected:
		std::string typeId = UnknownDirection::TypeId; 
		int id = UnknownDirection::Id; 		
		Quantity<Scalar,SIPrefix<0>,int> value;	
		PtrType tranferType;
	private:
	};
	
	class Direction: public DirectionBase<TransferIn,TransferOut,UnknownDirection>
	{
	public:
		using Base = DirectionBase<TransferIn,TransferOut,UnknownDirection>;
		Direction(std::string s): Base(s){ };
		Direction(): DirectionBase(""){ };
		Direction(const Bank::Direction&) = default;
	};
	
	std::ostream& operator<<(std::ostream& out, const Direction& d)	{	return d.Display(out);	}
}


#endif
