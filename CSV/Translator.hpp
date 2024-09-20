#include <iostream>
#include <fstream>
#include <vector>
#include "../Common/Color.hpp"
#include "../String/String_.hpp"

#pragma once

struct German		{   static constexpr const char* Identifier ="German"; };

template<typename T>
class IClone
{   
	public:
		auto Clone() const  { return clone(); };
	private:
		virtual std::unique_ptr<T> clone() const  = 0;
};

class ITranslate: public IClone<ITranslate>
{   
	public:
		const auto& operator()() const {	return get();	}
		bool operator==(const std::string& s) const {	return s == get();	}
	private:
		friend std::ostream& operator<<(std::ostream& out, const ITranslate& e) 	{	return out<<e.get();	}
		virtual const std::string& get() const = 0;
};

template<typename L>
class Translate: ITranslate
{   
	public:
		using Language = L::Identifier;
        Translate(const std::string& s): word{s} {   }
        virtual ~Translate(){  };
	private:
		std::unique_ptr<ITranslate> clone() const  { return std::make_unique<Translate<L>>(word); };
		const auto& get() const { return word; };
		std::string word;
};

class Line
{   
		static auto read(const std::string& s)
		{
			auto split = String_::Split(s,';');
			auto result = std::vector<std::string>();
			std::for_each(split.cbegin(), split.cend(), [&](const auto& t) { result.push_back(t); });
			return result;
		}
	public: 
        Line(const std::string& s): words{read(s)} {   }
        virtual ~Line(){  };
		bool operator==(const std::string& s) const {	return std::find(words.begin(), words.end(), s) != words.end();	}
		const auto& operator[](size_t i) const {	return words.at(i);	}
	private:
		friend std::ostream& operator<<(std::ostream& out, const Line& e) 
		{
			std::for_each(e.words.begin(), e.words.end(), [&](const auto& s) { out<<s<<"\t\t"; });
			return out<<"\n";
		}
		std::vector<std::string> words;
};

class Translator
{   
        const std::string fileName;
		int id = 1;
		std::unique_ptr<std::vector<Line>> translates;
		static auto read()
		{
			auto result = std::make_unique<std::vector<Line>>();
			result->push_back(Line("additional heating costs;Heiznebenkosten;HeatExtraCosts"));
			result->push_back(Line("extra costs;Nebenkosten;ExtraCosts"));
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
		friend std::ostream& operator<<(std::ostream& out, const Translator& e) 
		{
			std::for_each(e.translates->begin(), e.translates->end(), [&](const auto& s) { out<<s<<"\n"; });
			return out;
		}
    public: 
		const std::string& operator()(const std::string& s)
		{
			for(size_t i = 0;i < translates->size(); ++i)
			{
				if(translates->at(i)==s && id != -1)
					return translates->at(i)[id];
			}
			return s;
		}
        static Translator& Instance()
        {
            static Translator translator;
            return translator;
        };
};
