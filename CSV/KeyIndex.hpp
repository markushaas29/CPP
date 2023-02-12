#include <memory>
#include <limits>
#include <stdexcept>
#include <exception>
#include "Element.hpp"
#include "Elements.hpp"
#include "../Logger/Logger.hpp"
#include "../String/To/To.hpp"

#ifndef KEYINDEX_H
#define KEYINDEX_H

//--------------------------------KeyIndex------------------------------------------------
namespace CSV
{
	template<typename T = uint>
	class Index: public Element<Index<T>>
	{
			using Base = Element<Index<T>>;
			friend class Element<Index<T>>; 
			static constexpr uint max = std::numeric_limits<unsigned int>::max();
		public:
			inline static constexpr const char* Identifier = "Index";
			using ValueType = T;
			Index(ValueType v): Index{v,String_::ParseTo(v).c_str()}{};
			constexpr Index(ValueType v, const char* c): value{v}, Base{c}, valid{v != max}{};
			constexpr Index(): value{max}, Base{""}, valid{false}{};
			ValueType Get() const { return value; }
			bool Valid() const { return valid; }
			operator bool() const { return valid; }
			std::ostream& Display(std::ostream& os) const { return	os<<value;	}
		private:
			bool valid = false;
			ValueType value;
			static constexpr const char* check(const char* s) { return s; }
	};
	
	template<typename TIndexValue = uint>
	std::ostream& operator<<(std::ostream& strm, const Index<TIndexValue>& c) {	return c.Display(strm);	}
		
	template<typename T>
	inline decltype(auto) operator<=> (const Index<T>& lhs, const Index<T>& rhs){ return lhs.Get() <=> rhs.Get(); }

	template<typename T>
	inline bool operator== (const Index<T>& lhs, const Index<T>& rhs){ return lhs.Get() == rhs.Get(); }
	
	//~ template<typename AccountT,typename TKeyValue, typename TIndexValue> class KeyIndexContainer; //fwd
	
	template<typename TKeyValue = std::string, typename TIndexValue = uint>
	class KeyIndex
	{
			//~ template<typename A>
			//~ friend class KeyIndexContainer<A,TKeyValue,TIndexValue>; 
		public:
			using KeyType = Key<TKeyValue>;			
			using IndexType = Index<TIndexValue>;
			using ContainerType  = std::vector<KeyType>;
			using ContainerPtrType  = std::unique_ptr<ContainerType>;
			using Iterator  = std::vector<TKeyValue>::const_iterator;
			using KeyIndexType = KeyIndex<TKeyValue,TIndexValue>;
			KeyIndex(TKeyValue k): key{k}{};
			KeyIndex(Iterator begin, Iterator end){ setKeyPatterns(begin,end); };
			void SetKeyPatterns(Iterator begin, Iterator end) { setKeyPatterns(begin,end); }
			
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
					if(match(values.at(i)))
                    {
						index = IndexType{i};
						Logger::Log<Info>("Value: ",values.at(i), " at ", i);
                        return true;
                    }
                }
                 
                return false;
			}
			
			bool operator ==(const KeyType& k) const { return key == k;  }
			bool operator ()(const std::string& s) const { return key(s);}
			bool Valid() const { return index.Valid(); }
			operator bool() const { return index.Valid(); }
			const KeyType& GetKey() const { return this->key; }
			const IndexType& GetIndex() const { return this->index; }
		private:
			void setKeyPatterns(Iterator begin, Iterator end) { std::for_each(begin,end,[&](auto s) { this->keys->push_back(KeyType(s)); }); }
			bool match(const std::string& s)	{ return std::find_if(this->keys->cbegin(), this->keys->cend(), [&](auto p){ return p == s;}) != this->keys->cend(); }
			KeyType key;
			IndexType index{};
			ContainerPtrType keys = std::make_unique<ContainerType>();
	};
	template<typename TKeyValue = std::string, typename TIndexValue = uint>
	bool operator ==(const Key<TKeyValue>& k, const KeyIndex<TKeyValue,TIndexValue>& ki){ return ki == k; }
	
	template<typename TKeyValue = std::string, typename TIndexValue = uint>
	bool operator ==(const KeyIndex<TKeyValue,TIndexValue>& ki, const Key<TKeyValue>& k){ return ki == k; }
		
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
			
			bool Check(const std::vector<std::string>& values)
			{
				bool result = true;
				for(auto it = keyIndices->begin(); it != keyIndices->end(); ++it)
					if(!(it->Update(values)))
						result = false;
							
				return result;
			}
			
			void UpdateKeyPatterns(const TKeyValue& k, const std::vector<std::string>& patterns)
			{
				try
				{
					auto i = std::find(keyIndices->begin(), keyIndices->end(),k);
					if(i != keyIndices->cend())
						i->SetKeyPatterns(patterns.cbegin(),patterns.cend());
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
				for(auto it = keyIndices->begin(); it != keyIndices->end(); ++it)
					os<<*it;
					
				return os;		
			}
			
			IndexType Get(TKeyValue key) 
			{
				for(auto it = keyIndices->cbegin(); it != keyIndices->cend(); ++it)
					if((it->GetKey()).Value() == key)
						return (it->GetIndex()).Get();
					
				throw std::invalid_argument( "received negative value" );
			}
			
			IndexType operator[](TKeyValue key) {	return Get(key);	}
			
			bool Empty() { return keyIndices->size() == 0;}
		private:
			ContainerPtrType keyIndices;
	};	
	
	template<typename AccountT,typename TKeyValue = std::string, typename TIndexValue = uint>
	std::ostream& operator<<(std::ostream& strm, const KeyIndexContainer<AccountT,TKeyValue,TIndexValue> c)	{	return c.Display(strm);	}
}

#endif
