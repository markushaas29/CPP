#include "Elements.hpp"
#include "../Logger/Logger.hpp"
#include <memory>
#include <exception>
#include <stdexcept>

#ifndef KEYINDEX_H
#define KEYINDEX_H

//--------------------------------KeyIndex------------------------------------------------
namespace CSV
{
	template<typename T = uint>
	class Index
	{
		public:
			using ValueType = T;
			Index(ValueType v = ValueType{}): value{v}{};
			void setValue(ValueType v){ this->value = v; }
			ValueType Get() const { return this->value; }
			
			std::ostream& Display(std::ostream& os) const
			{
					os<<this->value;
					
				return os;		
			}
			
		private:
			ValueType value;
	};
	
	template<typename TIndexValue = uint>
	std::ostream& operator<<(std::ostream& strm, const Index<TIndexValue>& c)
	{
		return c.Display(strm);
	}
	
	template<typename TKeyValue = std::string, typename TIndexValue = uint>
	class KeyIndex
	{
		public:
			KeyIndex(TKeyValue k): key{k}{};
			using KeyType = Key<TKeyValue>;			
			using ContainerType  = std::vector<KeyType>;
			using CIterator  = std::vector<TKeyValue>::const_iterator;
			using ContainerPtrType  = std::unique_ptr<ContainerType>;
			using IndexType = Index<TIndexValue>;
			using KeyIndexType = KeyIndex<TKeyValue,TIndexValue>;
			using Iterator = typename KeyType::Iterator;
			bool Is(std::string s)	{ return std::find_if(this->keys->cbegin(), this->keys->cend(), [&](auto p){ return p == s;}) != this->keys->cend(); }
			//~ void setIndexValue(TIndexValue i) { this->index.setValue(i); }
			void setKeyPatterns(Iterator begin, Iterator end) { std::for_each(begin,end,[&](auto s) { this->keys->push_back(KeyType(s)); }); }
			
			std::ostream& Display(std::ostream& os) const
			{
				auto patterns = this->key.Patterns();
				os<<this->key<<": "<<std::endl;	
				for(auto it = patterns->cbegin(); it != patterns->cend(); ++it)
					os<<"\t"<<*it<<std::endl;
				os<<" Current: "<<this->key.Current()<<" at "<<this->index<<"\n"<<std::endl;	
				return os;		
			}
			
			bool Update(const std::vector<std::string>& values)
			{
				for(uint i = 0; i < values.size(); ++i)
				{
					if(this->Is(values.at(i)))
                    {
						index = IndexType{i};
						Logger::Log<Info>("Value: ",values.at(i), " at ", i);
                        return true;
                    }
                }
                 
                return false;
			}
			
			bool operator ==(const KeyType& k){ return this->key == k;  }
			bool operator ()(const std::string& s) { return this->key(s);}
			const KeyType& GetKey() const { return this->key; }
			const IndexType& GetIndex() const { return this->index; }
					
		private:
			KeyType key;
			IndexType index;
			ContainerPtrType keys = std::make_unique<ContainerType>();
	};
	template<typename TKeyValue = std::string, typename TIndexValue = uint>
	bool operator ==(const Key<TKeyValue>& k, const KeyIndex<TKeyValue,TIndexValue>& ki){ return ki == k; }
	
	template<typename TKeyValue = std::string, typename TIndexValue = uint>
	bool operator ==(const KeyIndex<TKeyValue,TIndexValue>& ki, const Key<TKeyValue>& k){ return ki == k; }
	
	//~ template<typename TKeyValue = std::string, typename TIndexValue = uint>
	//~ bool operator ==(const KeyIndex<TKeyValue,TIndexValue>& ki, const TKeyValue& kv){ return ki == Key<TKeyValue>(kv); }
	
	template<typename TKeyValue = std::string, typename TIndexValue = uint>
	std::ostream& operator<<(std::ostream& strm, const KeyIndex<TKeyValue,TIndexValue> c){	return c.Display(strm);	}
	
	template<typename AccountT,typename TKeyValue = std::string, typename TIndexValue = uint>
	class KeyIndexContainer
	{
		public:
			using KeyType = Key<TKeyValue>;
			using IndexType = TIndexValue;
			using AccountType = AccountT;
			using KeyIndexType = KeyIndex<TKeyValue,TIndexValue>;
			using ContainerType  = std::vector<KeyIndexType>;
			using ContainerPtrType  = std::unique_ptr<ContainerType>;
			KeyIndexContainer(ContainerPtrType k): keyIndices{std::move(k)} { };
			
			bool UpdateKeys(const std::vector<std::string>& values)
			{
				bool result = true;
				for(auto it = this->keyIndices->begin(); it != this->keyIndices->end(); ++it)
					if(!(it->Update(values)))
						result = false;
							
				return result;
			}
			
			void UpdateKeyPatterns(const KeyType&  k, const std::vector<std::string>& patterns)	{	this->UpdateKeyPatterns(k.Value(), patterns);	}
			
			void UpdateKeyPatterns(const TKeyValue& k, const std::vector<std::string>& patterns)
			{
				try
				{
					auto i = std::find(this->keyIndices->begin(),this->keyIndices->end(),k);
					if(i != this->keyIndices->cend())
						i->setKeyPatterns(patterns.cbegin(),patterns.cend());
					else
						Logger::Log<Error>()<<"UpdateKeyPatterns Key "<<k<<" not found!"<<std::endl;					
				}
				catch(std::exception e)
				{
					Logger::Log<Error>()<<"UpdateKeyPatterns Key "<<k<<"\t"<<e.what()<<std::endl;					
				}
					
			}
			
			std::ostream& Display(std::ostream& os) const
			{
				for(auto it = this->keyIndices->begin(); it != this->keyIndices->end(); ++it)
					os<<*it;
					
				return os;		
			}
			
			IndexType Get(TKeyValue key) 
			{
				for(auto it = this->keyIndices->cbegin(); it != this->keyIndices->cend(); ++it)
					if((it->GetKey()).Value() == key)
						return (it->GetIndex()).Get();
					
				throw std::invalid_argument( "received negative value" );
			}
			
			IndexType operator[](TKeyValue key) {	return this->Get(key);	}
			
			bool Empty() { return this->keyIndices->size() == 0;}
		private:
			ContainerPtrType keyIndices;
	};	
	
	template<typename AccountT,typename TKeyValue = std::string, typename TIndexValue = uint>
	std::ostream& operator<<(std::ostream& strm, const KeyIndexContainer<AccountT,TKeyValue,TIndexValue> c)	{	return c.Display(strm);	}
}

#endif
