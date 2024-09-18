#include <iostream>
#include <fstream>
#include <vector>
#include "../Common/Color.hpp"

#pragma once

//struct Debug: LogTypeBase<Debug, Default>{   static constexpr const char* Identifier ="[DEBUG]: "; };

class Translator
{   
        std::string fileName;
        Translator()
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
			return s;
		}
        static Translator& Instance()
        {
            static Translator translator;
            return translator;
        };
};
