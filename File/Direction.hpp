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
	class DirectionType
	{
	public:
		virtual const std::string& Sign() = 0;	
	};
	
	class TransferIn: public DirectionType
	{
	public:
		using Type = TransferIn;
		using SignType = T::char_<'+'>;
		inline static const std::string TypeId = "TransferIn"; 
		inline static constexpr int Id = 1; 
		inline static constexpr char SignValue = SignType::Value; 
		virtual const std::string& Sign() { return SignType::String; };		
	};
	
	class TransferOut: public DirectionType
	{
	public:
		using Type = TransferOut;
		using SignType = T::char_<'-'>;
		inline static const std::string TypeId = "TransferOut"; 
		inline static constexpr int Id = -1; 
		inline static constexpr char SignValue = SignType::Value; 
		virtual const std::string& Sign() { return SignType::String; };		
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
	};
	
	template<typename TIn=TransferIn, typename TOut=TransferOut, typename TUnknown=UnknownDirection>
	class DirectionBase: public Element
	{
	public:
		DirectionBase(std::string s): Element(s), tranferType{Create(*(s.cbegin()))},value(UnknownDirection::Id), id{UnknownDirection::Id}, typeId{UnknownDirection::TypeId} { Logger::Log()<<"DIRECTION: "<<*(s.cbegin())<<std::endl; };
		using Type = DirectionBase;
		using QuantityType = Quantity<Scalar,SIPrefix<0>>;
		using PtrType = std::shared_ptr<DirectionType>;
		
		inline static constexpr const char* Identifier = "Direction";
		
		DirectionBase* DoCreate(){return this;};
		DirectionBase(const DirectionBase&) = default;
		const auto& Value() const {	return this->value; }
		const auto& Id() const  {	return this->id; }
		const auto& TypeId() const  {	return this->typeId; }	
	
		static PtrType Create(const char c)
		{
			if(c == 'H')
				return std::make_unique<TransferIn>();
			if(c == 'S')
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
		
		template<typename AccountT>
		void Update()
		{
			this->id = AccountT::Id;
			this->typeId = AccountT::TypeId;
			this->value = QuantityType(AccountT::Id);
			
			Logger::Log()<<"SET______"<<this->id<<"\t"<<this->typeId<<"\t"<<this->value<<std::endl;
		}
		
		
		template<typename DirectionT>
		void Set()
		{
			this->id = DirectionT::Id;
			this->typeId = DirectionT::TypeId;
			this->value = QuantityType(DirectionT::Id);
		}
	};
	
	std::ostream& operator<<(std::ostream& out, const Direction& d)
	{
		return d.Display(out);		
	}
}


#endif
