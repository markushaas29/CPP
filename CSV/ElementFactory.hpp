#include "Element.hpp"
#include "../String/String_.hpp"
#include "../Logger/Logger.hpp"
#include "../Quantity/Quantity.hpp"
#include <boost/mpl/for_each.hpp>
#include <boost/mpl/vector.hpp>
#include <string.h>
#include <map>
#include <memory>
#include <array>
#include <chrono>
#include <ctime>

#ifndef ELEMENTFACTORY_H
#define ELEMENTFACTORY_H

//~ template<typename T = double>
//~ class Value: public Element
//~ {
	//~ using ConverterT = String_::From<T>;
	//~ ConverterT converter = ConverterT();
	
	//~ T value;
	//~ T Parse(std::string s){ return std::stod(s); }
//~ public:
	//~ using Type = T;
	
	//~ inline static const std::string Identifier = "Value";
	//~ Value(std::string s = ""): Element(s.c_str()), value(this->Parse(s)){};
	//~ Value(T val, std::string s = ""): Element(this->converter(val)), value(val){};
	//~ Value* DoCreate(){return this;};
	//~ T Get(){ return this->value; }
//~ };

//--------------------------------Factory------------------------------------------------
class Date;

//~ using Elements = boost::mpl::vector<Key<std::string>, Value<double>, Entry, Date>;

//~ template<class TList = Elements, class Unit = Element,typename T = std::string,class IdentifierType = std::string, template<class> class CreatePolicy = CreateElementNewPolicy>
//~ class ElementFactory
//~ {
	//~ using ProductList = TList;
	//~ using Creator = Unit* (*)(T);
	
//~ private:
	//~ using AssocMap = std::map<IdentifierType,Creator>;
	//~ inline static AssocMap associations_ = std::map<IdentifierType,Creator>();
	
	//~ struct Register
	//~ {
		//~ template<class Type>
		//~ void operator()(Type) const
		//~ {
			//~ RegisterImpl(Type::Identifier, CreatePolicy<Type>::DoCreate);
		//~ };
	//~ };
	
	//~ static bool RegisterImpl(const IdentifierType& id, const Creator& creator)
	//~ {
		//~ return associations_.insert(std::make_pair(id,creator)).second;
	//~ }
	
//~ public:
	//~ ElementFactory()
	//~ {		
		//~ boost::mpl::for_each<ProductList>(Register());
	//~ }
	
	
	//~ bool Unregister(const IdentifierType& id)
	//~ {
		//~ return associations_.erase(id) == 1;
	//~ }
		
	//~ Unit* Create(const IdentifierType& id,T param)
	//~ {
		//~ typename AssocMap::const_iterator i = associations_.find(id);
		
		//~ if(i != associations_.end())
		//~ {
			//~ return (i->second)(param);
		//~ }
		
		//~ return (associations_.find(id)->second)(param);
	//~ }
//~ };

#endif
