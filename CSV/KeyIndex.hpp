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
			explicit Index(ValueType v): Index{v,String_::ParseTo(v).c_str()}{};
			explicit Index(int v): Index{(T)v}{};
			constexpr Index(ValueType v, const char* c): value{v}, Base{c}, valid{v != max}{};
			constexpr Index(): value{max}, Base{""}, valid{false}{};
			ValueType Get() const { return value; }
			bool Valid() const { return valid; }
			explicit operator bool() const { return valid; }
			explicit operator int() const { return static_cast<int>(value); }
			std::ostream& Display(std::ostream& os) const { return	os<<value;	}
		private:
			bool valid = false;
			ValueType value;
			static constexpr const char* check(const char* s) { return s; }
			static constexpr bool isValid(ValueType v) { return v != max; }
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
			inline constexpr static const char* Identifier="KeyIndex";
			//~ KeyIndex(const TKeyValue& v): identifier{v}, key{v.Value()}, index{0} { keys->push_back(); };
			explicit KeyIndex(const KeyType& k): stringId{k.Value()}, key{k}, index{TIndexValue{0}} { keys->push_back(k); };
			KeyIndex(Iterator begin, Iterator end){ setKeyPatterns(begin,end); };
			KeyIndex(const KeyIndex& k): keys{std::make_unique<ContainerType>(*k.keys)}, key{k.key}, index{k.index}{ }
			void SetKeyPatterns(Iterator begin, Iterator end) { setKeyPatterns(begin,end); }
			
			std::ostream& Display(std::ostream& os) const
			{
				os<<" Current: "<<key<<" at "<<index<<"\n Keys:";	
				for(auto it = keys->cbegin(); it != keys->cend(); ++it)
					os<<"\t"<<*it<<", ";
				return os<<"\n";		
			}
			
			bool Check(const std::vector<std::string>& values)
			{
				for(uint i = 0; i < values.size(); ++i)
					if(HasKey(values.at(i)))
						return updateIndex(values,i);
                 
                return false;
			}
			
			bool operator ()(const std::string& s) const { return key(s);}
			bool operator ==(const KeyType& k) const { return key == k;  }
			explicit operator bool() const { return valid; }
			bool Valid() const { return valid; }
			decltype(auto) KeysSize() const { return keys->size(); }
			const TKeyValue& StringId() const { return stringId; }
			const KeyType& GetKey() const { return key; }
			const IndexType& GetIndex() const { return index; }
            bool HasKey(const std::string& s) const { return std::find_if(keys->cbegin(), keys->cend(), [&](auto p){ return p == s;}) != keys->cend(); }
            template<typename It>
            bool HasKey(It begin, It end) const { return std::find_if(begin,end, [&](auto s){ return HasKey(s);}) != end; }
		private:
			bool updateIndex(const auto& values, const TIndexValue i)
			{
				index = IndexType{static_cast<TIndexValue>(i)};
				key = keys->at(i);
				valid = true;
				Logger::Log<Info>("KeyIndex: set key: ",key, " at ", index);
				return true;
            }
            
            void setKeyPatterns(Iterator begin, Iterator end) 
            { 
				if(begin==end)
					throw "KeyINdex: setKeyPatterns => begin == end";
				std::for_each(begin,end,[&](auto s) { keys->push_back(KeyType(s)); }); 
				key = keys->at(0);
				index = Index{TIndexValue{0}};
			}
            bool match(const std::string& s)   { return std::find_if(keys->cbegin(), keys->cend(), [&](auto p){ return p == s;}) != keys->cend(); }

			const TKeyValue stringId="KeyIndex";
			bool valid{};
			KeyType key{};
			IndexType index{};
			ContainerPtrType keys = std::make_unique<ContainerType>();
	};
	template<typename TKeyValue = std::string, typename TIndexValue = uint>
	bool operator ==(const Key<TKeyValue>& k, const KeyIndex<TKeyValue,TIndexValue>& ki){ return ki == k; }
	
	template<typename TKeyValue = std::string, typename TIndexValue = uint>
	bool operator ==(const KeyIndex<TKeyValue,TIndexValue>& ki, const Key<TKeyValue>& k){ return ki == k; }
		
	template<typename TKeyValue = std::string, typename TIndexValue = uint>
	std::ostream& operator<<(std::ostream& strm, const KeyIndex<TKeyValue,TIndexValue>& c){	return c.Display(strm);	}
	
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
			explicit KeyIndexContainer(ContainerPtrType k): keyIndices{std::move(k)} {};
			
			bool Check(const std::vector<std::string>& values)
			{
				bool result = true;
				for(auto it = keyIndices->begin(); it != keyIndices->end(); ++it)
					if(!(it->Check(values)))
						result = false;
				return result;
			}
			
			void UpdateKeyPatterns(const TKeyValue& k, const std::vector<std::string>& patterns)
			{
				try
				{
					auto i = std::find_if(keyIndices->begin(), keyIndices->end(),[&](auto ki){ return ki.GetKey().Value()==k; });				
					if(i != keyIndices->cend())
							i->SetKeyPatterns(patterns.cbegin(),patterns.cend());
					else
						Logger::Log<Error>("UpdateKeyPatterns Key ",k," not found!");					
				}
				catch(std::exception e)
				{
					Logger::Log<Error>()<<"UpdateKeyPatterns Key "<<k<<"\t"<<e.what()<<std::endl;					
				}
					
			}
			
			std::ostream& Display(std::ostream& os) const
			{
				for(auto it = keyIndices->begin(); it != keyIndices->end(); ++it)
					it->Display(os);
					
				return os;		
			}
			
			IndexType Get(TKeyValue key) 
			{
				for(auto it = keyIndices->cbegin(); it != keyIndices->cend(); ++it)
					if((it->GetKey()).Value() == key)
						return (it->GetIndex()).Get();
					
				throw std::invalid_argument( "received negative value" );
			}
			
			void Add(KeyIndexType&& ki) { keyIndices->push_back(std::move(ki)); }
			IndexType operator[](TKeyValue key) {	return Get(key);	}
			
			bool Empty() { return keyIndices->size() == 0;}
		private:
			ContainerPtrType keyIndices;
	};	
	
	template<typename AccountT,typename TKeyValue = std::string, typename TIndexValue = uint>
	std::ostream& operator<<(std::ostream& strm, const KeyIndexContainer<AccountT,TKeyValue,TIndexValue>& c)	{	return c.Display(strm);	}
}

#endif
