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
		std::ostream& Display(std::ostream& os) const{ return Base::Display(os)<<"\n"<<Type::Identifier;}
		void Read(const std::string& sourcePath = "."){	Base::Read();	}
		
		template<typename TransferType>
		auto CreateTransfer(Base::InputIterator begin, Base::InputIterator end)
		{
			std::cout<<Type::Identifier<<"\n";
			auto bT = Base::createTransfer(begin,end);
			auto result = std::tuple_cat(bT, TupleType(Type(*(begin + (*(this->keyIndices))[Type::Identifier]))));
			typename TransferType::TupleType t = result;
			
			auto tt = TransferType(std::move(t));
						
			return std::make_shared<TransferType>(std::move(tt));
		}
		
		static TransferItemContainer& Instance()
		{
			static TransferItemContainer instance;
			return instance;
		}	
	protected:
		TransferItemContainer() 
		{ 
			std::cout<<Type::Identifier<<"\n";
			auto ki = typename KeyIndexContainerType::KeyIndexType(Type::Identifier);
			this->keyIndices->Add(std::move(ki));
			Logger::Log<Info>("TransferItemContainer created.");
		};
		
		auto createTransfer(Base::InputIterator begin, Base::InputIterator end) 
		{
			auto bT = Base::createTransfer(begin,end); 
			auto result = std::tuple_cat(bT, TupleType(Type(*(begin + (*(this->keyIndices))[Type::Identifier]))));
			return result;
		} 
};

template<typename KeyIndexContainerType, typename Tuple>
class TransferItemContainer<KeyIndexContainerType, Tuple, 0>
{
	public:
		using Type = std::tuple_element_t<0, Tuple>;
		using InputIterator = std::vector<std::string>::const_iterator;
		using ContainerType = TransferItemContainer<KeyIndexContainerType,Tuple,0>;
		using KeyIndexContainerPtrType = std::unique_ptr<KeyIndexContainerType>;
		using KeyIndexType = typename KeyIndexContainerType::KeyIndexType;
		using TupleType = std::tuple<Type>;
		static constexpr uint Index = 0;
		void UpdateKeyIndices(InputIterator begin, InputIterator end)
		{
			if(begin==end)
			{
	 			Logger::Log<Error>("ReadKeyPatterns: No Items found for ");
	 			// Todo weiter Fehlerbehandlung für weiter Bearbeitung
	 			return;
			}
						
			for(auto it = begin; it != end; ++it)
			{
				try
				{
					auto values = String_::Split<T::char_<':'>>(*it);
					auto keyItem = *values.cbegin();
					auto keys = String_::Split<T::char_<';'>>(String_::Remove<String_::CR>(*(values.cbegin()+1)));
					
		 			if(keys.cbegin() != keys.cend())
		 			{
			 			Logger::Log<Info>(" ReadKeyPatterns: Key found for item: ",keyItem);
						keyIndices->UpdateKeyPatterns(keyItem, keys);
					}
				}
				catch(std::exception e)
				{
		 			Logger::Log<Error>("ReadKeyPatterns: \t",e.what());
				}
			}
		}
		
		bool Check(const std::vector<std::string>& values) { return keyIndices->Check(values); }
		bool Empty() const { return keyIndices->Empty(); }
	
		std::ostream& Display(std::ostream& os) const { return os<<Type::Identifier<<"\nKeyIndices:\n"<<*keyIndices<<"\n";	}
		void Read(const std::string& sourcePath = "."){	}

		static TransferItemContainer& Instance()
		{
			static TransferItemContainer instance;
			return instance;
		}	
	protected:
		TransferItemContainer() 
		{ 
			using Cont = KeyIndexContainerType::ContainerType;
			auto cont = std::make_unique<Cont>();
			cont->push_back(KeyIndexType(Type::Identifier));
			keyIndices = std::make_unique<KeyIndexContainerType>(std::move(cont));
			Logger::Log<Info>()<<"TransferItemContainer created."<<std::endl; 
		};
		KeyIndexContainerPtrType keyIndices;
		auto createTransfer(InputIterator begin, InputIterator end) { return TupleType(Type(*(begin + (*(this->keyIndices))[Type::Identifier])));	}
};

template<typename KeyIndexContainerType, typename Tuple, uint Idx>
std::ostream& operator<<(std::ostream& strm, const TransferItemContainer<KeyIndexContainerType,Tuple, Idx>& c){	return c.Display(strm);}

#endif
