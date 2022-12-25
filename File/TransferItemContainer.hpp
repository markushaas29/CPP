#include <algorithm>
#include <functional>
#include <iostream>
#include <fstream>
#include <chrono>
#include <tuple>
#include <iterator>
#include <vector>
#include <filesystem>
#include "Transfer.hpp"
#include "../CSV/Elements.hpp"
#include "../Unit/Unit.hpp"
#include "../Quantity/Quantity.hpp"
#include "../Common/DateTimes.hpp"
#include "../Logger/Logger.hpp"
#include "../Typelist/Typelist.h"

#ifndef TRANSFERITEMCONTAINER_HPP
#define TRANSFERITEMCONTAINER_HPP

//---------------------------------------------------------------------------------------------------CounterContainer----------------------------------------------------------------------------------------

template<typename KeyIndexContainerType, typename Tuple, uint Idx = std::tuple_size_v<Tuple> - 1>
class TransferItemContainer: public TransferItemContainer<KeyIndexContainerType,Tuple, Idx-1>
{
	public:
		using Type = std::tuple_element_t<Idx, Tuple>;
		using ContainerType = TransferItemContainer<KeyIndexContainerType,Tuple, Idx>;
		using Base = TransferItemContainer<KeyIndexContainerType, Tuple, Idx-1>;
		using TupleType = std::tuple<Type>;
		static constexpr uint Index = Idx;
	protected:
		TransferItemContainer() { Logger::Log<Info>()<<"TransferItemContainer created."<<std::endl; };
		
		auto createTransfer(Base::InputIterator begin, Base::InputIterator end) 
		{
			auto bT = Base::createTransfer(begin,end); 
			auto result = std::tuple_cat(bT, TupleType(Type(*(begin + (*(this->keyIndices))[Type::Identifier]))));
			return result;
		} 
		
		template<typename T, typename Cont = T::KeyIndexContainerType::ContainerType>
		auto Create(const std::string& sourcePath, Cont ret)
		{
			ret->push_back(typename T::KeyIndexContainerType::KeyIndexType(Type::Identifier));
			return Base::template Create<T>(sourcePath, std::move(ret));	
		}
	public:
		static std::ostream& Display(std::ostream& os) 	{ return Base::Display(Type::Display(os));}
		void Read(const std::string& sourcePath = "."){	Base::Read();	}
		
		template<typename TransferType>
		auto CreateTransfer(Base::InputIterator begin, Base::InputIterator end)
		{
			auto bT = Base::createTransfer(begin,end);
			auto result = std::tuple_cat(bT, TupleType(Type(*(begin + (*(this->keyIndices))[Type::Identifier]))));
			typename TransferType::TupleType t = result;
			
			auto tt = TransferType(std::move(t));
						
			return std::make_shared<TransferType>(std::move(tt));
		}
		
		template<typename T>
		auto Create(const std::string& sourcePath = ".")
		{
			auto ret = std::make_unique<typename T::KeyIndexContainerType::ContainerType>();
			ret->push_back(typename T::KeyIndexContainerType::KeyIndexType(Type::Identifier));
			return Base::template Create<T>(sourcePath, std::move(ret));		
		}
		
		static TransferItemContainer& Instance()
		{
			static TransferItemContainer instance;
			return instance;
		}	
};

template<typename KeyIndexContainerType, typename Tuple>
class TransferItemContainer<KeyIndexContainerType, Tuple, 0>
{
	public:
		using Type = std::tuple_element_t<0, Tuple>;
		using InputIterator = std::vector<std::string>::const_iterator;
		using ContainerType = TransferItemContainer<KeyIndexContainerType,Tuple,0>;
		using KeyIndexContainerPtrType = std::shared_ptr<KeyIndexContainerType>;
		using TupleType = std::tuple<Type>;
		static constexpr uint Index = 0;
	protected:
		TransferItemContainer() { Logger::Log<Info>()<<"TransferItemContainer created."<<std::endl; };
		KeyIndexContainerPtrType keyIndices;
		auto createTransfer(InputIterator begin, InputIterator end) {	return TupleType(Type(*(begin + (*(this->keyIndices))[Type::Identifier])));	}
		
		template<typename T, typename Cont = T::KeyIndexContainerType::ContainerType>
		auto Create(const std::string sourcePath, Cont ret)
		{
			ret->push_back(typename T::KeyIndexContainerType::KeyIndexType(Type::Identifier));
			return ret;		
		}
	public:
		static std::ostream& Display(std::ostream& os) { return Type::Display(os);	}
		void Read(const std::string& sourcePath = "."){	}
		void setKeyIndexContainer(KeyIndexContainerPtrType ptr){ this->keyIndices = ptr; }

		static TransferItemContainer& Instance()
		{
			static TransferItemContainer instance;
			return instance;
		}	
};

template<typename KeyIndexContainerType, typename Tuple, uint Idx>
std::ostream& operator<<(std::ostream& strm, const TransferItemContainer<KeyIndexContainerType,Tuple, Idx> c){	return c.Display(strm);}

#endif
