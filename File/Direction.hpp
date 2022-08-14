#include <algorithm>
#include <functional>
#include <iostream>
#include <fstream>
#include <memory>
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
#include "TransferContainer.hpp"
#include "../Logger/Logger.hpp"
#include "../CSV/CSV.hpp"
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
	class DirectionTypeBase
	{
	};
	
	class TransferIn: DirectionTypeBase
	{
	public:
		using Type = TransferIn;
		inline static const std::string TypeId = "TransferIn"; 
		inline static constexpr int Id = 1; 
	};
	
	class TransferOut: DirectionTypeBase
	{
	public:
		using Type = TransferOut;
		inline static const std::string TypeId = "TransferOut"; 
		inline static constexpr int Id = -1; 
	};
	
	class UnknownDirection: DirectionTypeBase
	{
	public:
		using Type = UnknownDirection;
		inline static const std::string TypeId = "UnknownDirection"; 
		inline static constexpr int Id = 0; 
	};
	
	template<typename TIn=TransferIn, typename TOut=TransferOut, typename TUnknown=UnknownDirection>
	class DirectionBase: public Element
	{
	public:
		DirectionBase(std::string s): Element(s), sign{*(s.cbegin())},value(UnknownDirection::Id), id{UnknownDirection::Id}, typeId{UnknownDirection::TypeId} { Logger::Log()<<"DIRECTION: "<<*(s.cbegin())<<std::endl; };
		using Type = DirectionBase;
		using QuantityType = Quantity<Scalar,SIPrefix<0>>;
		inline static constexpr const char* Identifier = "Direction";
		DirectionBase* DoCreate(){return this;};
		const auto& Value() const {	return this->value; }
		const auto& Id() const  {	return this->id; }
		const auto& TypeId() const  {	return this->typeId; }	
	
	protected:
		std::string typeId = UnknownDirection::TypeId; 
		int id = UnknownDirection::Id; 		
		Quantity<Scalar,SIPrefix<0>,int> value;	
		char sign;
	private:
	};
	
	class Direction: public DirectionBase<TransferIn,TransferOut,UnknownDirection>
	{
		using Base = DirectionBase<TransferIn,TransferOut,UnknownDirection>;
		Direction(std::string s): Base(s){ };
		Direction(): DirectionBase(""){ };
		
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
}


#endif
