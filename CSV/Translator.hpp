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
			auto result = std::make_unique<std::vector<std::string>>();
			result->push_back("additional heating costs");
			result->push_back("Heiznebenkosten");
			result->push_back("HeatExtraCosts");
			return result;
		}
	public: 
        Line(): words{read()}
        {
			std::cout<<"Line"<<std::endl;
 //           if(!this->file.good())
 //                throw std::ios_base::failure(this->logFileName);
        }
		bool operator==(const std::string& s) const {	return std::find(words->begin(), words->end(), s) != words->end();	}
	private:
        const std::string fileName;
		int id = 1;
		std::unique_ptr<std::vector<std::string>> words;
        virtual ~Line(){  };
};
class Translator
{   
        const std::string fileName;
		int id = 1;
		std::unique_ptr<std::vector<std::vector<std::string>>> translates;
		static auto read()
		{
			auto result = std::make_unique<std::vector<std::vector<std::string>>>();
			result->push_back({"additional heating costs", "Heiznebenkosten", "HeatExtraCosts"});
			result->push_back({"extra costs", "Nebenkosten", "ExtraCosts"});
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
    public: 
		const std::string& operator()(const std::string& s)
		{
			for(size_t i = 0;i < translates->size(); ++i)
			{
				auto line = translates->at(i);
				if(auto t = std::find(line.begin(), line.end(), s) != line.end() && id != -1)
					return line.at(id);
			}
			return s;
		}
        static Translator& Instance()
        {
            static Translator translator;
            return translator;
        };
};
