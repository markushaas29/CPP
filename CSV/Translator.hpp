#include <iostream>
#include <fstream>
#include <vector>
#include "../Common/Color.hpp"
#include "../Common/IClone.hpp"
#include "../String/String_.hpp"
#include "../ObjectFactory/Factory.hpp"

#pragma once

struct German		{   static constexpr const char* Identifier ="German"; };

class ITranslate: public IClone<ITranslate>
{   
	public:
		const auto& operator()() const {	return get();	}
		const auto& Get() const {	return get();	}
		bool operator==(const std::string& s) const {	return s == get();	}
	private:
		friend std::ostream& operator<<(std::ostream& out, const ITranslate& e) 	{	return out<<e.get();	}
		virtual const std::string& get() const = 0;
};

struct Translator;

template<typename L>
class Translate: public ITranslate
{   
	public:
		inline static const std::string Language = L::Identifier;
        Translate(const std::string& s): word{s} {   }
        virtual ~Translate(){  };
		template<typename T>
        static inline void instance() { T::instance().factory; }
	private:
		friend class Translator;
		std::unique_ptr<ITranslate> clone() const  { return std::make_unique<Translate>(word); };
		const std::string& get() const { return word; };
		std::string word;
};

class Line
{   
		static auto read(const std::string& s)
		{
			auto split = String_::Split(s,';');
			auto result = std::make_unique<std::vector<std::unique_ptr<ITranslate>>>();
			std::for_each(split.cbegin(), split.cend(), [&](const auto& t) { result->push_back(std::make_unique<Translate<German>>(t)); });
			return result;
		}
	public: 
        Line(const std::string& s): translates{read(s)} {   }
        virtual ~Line(){  };
		bool operator==(const std::string& s) const {	return std::find_if(translates->begin(), translates->end(), [&s](const auto& w) { return w->Get() == s; }) != translates->end();	}
		const auto& operator[](size_t i) const {	return translates->at(i)->Get();	}
	private:
		friend std::ostream& operator<<(std::ostream& out, const Line& e) 
		{
			std::for_each(e.translates->begin(), e.translates->end(), [&](const auto& s) { out<<*s<<"\t\t"; });
			return out<<"\n";
		}
		std::unique_ptr<std::vector<std::unique_ptr<ITranslate>>> translates;
};

class Translator
{   
		friend class Line;
		template<typename> friend class Translate;
        const std::string fileName;
		int id = 1;
		std::unique_ptr<std::vector<std::unique_ptr<Line>>> translates;
		Factory<ITranslate> factory;
		
		static auto read()
		{
			auto result = std::make_unique<std::vector<std::unique_ptr<Line>>>();
			result->push_back(std::make_unique<Line>("additional heating costs;Heiznebenkosten;HeatExtraCosts"));
			result->push_back(std::make_unique<Line>("extra costs;Nebenkosten;ExtraCosts"));
			return result;
		}
        Translator(): translates{read()}
        {
			std::cout<<"Translator"<<std::endl;
 //           if(!this->file.good())
 //                throw std::ios_base::failure(this->logFileName);
        }
        Translator(const Translator&) = delete;
        void operator=(const Translator&) = delete;
        virtual ~Translator(){  };
        static Translator& instance()
        {
            static Translator translator;
            return translator;
        };
		friend std::ostream& operator<<(std::ostream& out, const Translator& e) 
		{
			std::for_each(e.translates->begin(), e.translates->end(), [&](const auto& s) { out<<*s<<"\n"; });
			return out;
		}
    public: 
		const std::string& operator()(const std::string& s)
		{
			for(size_t i = 0;i < translates->size(); ++i)
			{
				if(*(translates->at(i))==s && id != -1)
					return (*translates->at(i))[id];
			}
			return s;
		}
        static Translator& Instance()
        {
            auto& translator = instance();
			Translate<German>::instance<Translator>();
            return translator;
        };
};
