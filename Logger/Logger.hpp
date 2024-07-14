#include <algorithm>
#include <functional>
#include <iostream>
#include <fstream>
#include <iterator>
#include <vector>
#include <cstdlib>
#include <unordered_map>
#include <cassert>
#include <sstream>
#include <string_view>
#include <source_location>
#include "LogExpression.hpp"
#include "../Common/Color.hpp"
#include "../Wrapper/Wrapper.hpp"

#ifndef LOGGER_HPP
#define LOGGER_HPP


template<typename Derived, typename T>
struct LogTypeBase
{
	static constexpr uint color{T::FG};
	static constexpr Color::Modifier<T> mod{}; 
	static std::ostream& Log(std::ostream& os)	{ return os<<mod<<Derived::Identifier;	};
};

struct Debug: LogTypeBase<Debug, Color::Default>{   static constexpr const char* Identifier ="[DEBUG]: "; };
struct Info: LogTypeBase<Info, Color::Default>{    static constexpr const char* Identifier ="[Info]: "; };
struct Success: LogTypeBase<Success, Color::Green>{    static constexpr const char* Identifier ="[Success]: "; };
struct Warning: LogTypeBase<Warning, Color::Yellow>{    static constexpr const char* Identifier ="[Warning]: ";};
struct Error: LogTypeBase<Error, Color::Red>{    static constexpr const char* Identifier ="[ERROR]: ";};

class Logger
{   
	public:
        std::ofstream file;
        std::ostream& out = std::cout;
    private:
        std::string logFileName;
        Logger()
        {
            out<<"Logger started"<<std::endl;
            this->logFileName = "Logging.txt";
			this->file.clear();
            this->file.open(this->logFileName);
            if(!this->file.good())
                 throw std::ios_base::failure(this->logFileName);
        }
        Logger(const Logger&) = delete;
        void operator=(const Logger&) = delete;
        virtual ~Logger(){ this->file.close();  };
        
        template<typename T>
        static std::ostream& log(std::ostream& os,T t) { return os<<" "<<t<<Color::Modifier<Color::Default>()<<std::endl;   };
        
        template<typename T, typename ...Ts>
        static std::ostream& log(std::ostream& os,T t, Ts ...ts)
        {
			os<<" "<<t;
			return log(os,ts...);
        };
    public: 
        static Logger& Instance()
        {
            static Logger logger;
            return logger;
        };
        
		static std::string Source(const std::string& message, const std::source_location location = std::source_location::current())
		{
			std::stringstream result;
			result<<"file: " << location.file_name() << "("  << location.line() + ":" <<  location.column() << ") `" << location.function_name() << "`: " << '\n';
			return result.str();
		}

        template<class LogPolicy = Debug>
        static std::ostream& Log() {	return LogPolicy::Log(Logger::Instance().file);    };
        
        template<class LogPolicy = Debug, typename ...Vals>
        static std::ostream& Log(Vals ...vals)
        {
			if constexpr (!std::is_same<Debug,LogPolicy>::value )
				log(LogPolicy::Log(Logger::Instance().out),vals...);

			return log(LogPolicy::Log(Logger::Instance().file),vals...);
        };
        
        template<char C, class LogPolicy = Debug, typename ...Vals>
        static std::ostream& Log(Vals ...vals)
        {
			if constexpr (std::is_same<Debug,LogPolicy>::value)
				return log(LogPolicy::Log(Logger::Instance().file),vals...);

			return log(LogPolicy::Log(Logger::Instance().out),vals...);
        };
		
        template<typename Iterator,class LogPolicy = Debug>
        static void Log(Iterator begin, Iterator end, std::string name = "")
        {
			int j = 0;

			//~ if(name != "")
				//~ Logger::Log()<<name<<" :"<<std::endl;
			
			//~ for(auto i = begin; i != end; ++i, ++j)
				//~ Logger::Log<LogPolicy>()<<"\t"<<j<<" :"<<*i<<std::endl;
		};		
};

#ifndef NDEBUG
#   define LOG(message) Logger::Log()<<"File: "<<__FILE__ << " line " << __LINE__ << ": " << message << std::endl; 
#else
#   define LOG(message) 
#endif

template<typename T>
struct DebugDeleter {  void operator()(T* t)  {	/*Logger::Log()<<"Pointer deleted."<<std::endl;*/ } };

#endif
