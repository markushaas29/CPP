#include <iostream>
#include <fstream>
#include <vector>
#include "../Common/Color.hpp"

#pragma once

//struct Debug: LogTypeBase<Debug, Default>{   static constexpr const char* Identifier ="[DEBUG]: "; };

class Line
{   
		static auto read()
		{
			auto result = std::vector<std::string>();
			result.push_back("additional heating costs");
			result.push_back("Heiznebenkosten");
			result.push_back("HeatExtraCosts");
			return result;
		}
	public: 
        Line(): words{read()} {   }
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
			result->push_back(Line());
			result->push_back(Line());
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
