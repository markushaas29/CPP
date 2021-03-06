#include <algorithm>
#include <functional>
#include <iostream>
#include <fstream>
#include <iterator>
#include <vector>
#include <cstdlib>
#include <unordered_map>
#include <cassert>
#include "LogExpression.hpp"
#include "../Wrapper/Wrapper.hpp"

#ifndef NDEBUG
#   define ASSERT(condition, message) \
    do { \
        if (! (condition)) { \
            std::cerr << "Assertion `" #condition "` failed in " << __FILE__ \
                      << " line " << __LINE__ << ": " << message << std::endl; \
            std::terminate(); \
        } \
    } while (false)
#else
#   define ASSERT(condition, message) do { } while (false)
#endif


#ifndef LOGGER_HPP
#define LOGGER_HPP

template<typename Derived>
struct LogTypeBase
{
	static std::ostream& Log(std::ostream& os)
	{
		return os<<Derived::Identifier;
	};
};

struct Debug: LogTypeBase<Debug>
{
    static constexpr const char* Identifier ="[DEBUG]: ";
};

struct Info: LogTypeBase<Info>
{
    static constexpr const char* Identifier ="[Info]: ";
};

struct Error: LogTypeBase<Debug>
{
    static constexpr const char* Identifier ="[ERROR]: ";
};

class Logger
{   public:
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
        static std::ostream& log(std::ostream& os,T t)
        {
			return os<<" "<<t<<std::endl;
        };
        
        template<typename T, typename ...Ts>
        static std::ostream& log(std::ostream& os,T t, Ts ...ts)
        {
			os<<" "<<t;;
			return log(os,ts...);
        };
    public: 
        static Logger& Instance()
        {
            static Logger logger;
            return logger;
        };

        template<class LogPolicy = Debug>
        static std::ostream& Log()
        {
				return LogPolicy::Log(Logger::Instance().file);
			//~ #ifndef NDEBUG
			//~ #else
				//~ return LogPolicy::Log(Logger::Instance().out);
			//~ #endif
        };
        
        template<class LogPolicy = Debug, typename ...Vals>
        static std::ostream& Log(Vals ...vals)
        {
			if constexpr (std::is_same<Debug,LogPolicy>::value)
				return log(LogPolicy::Log(Logger::Instance().file),vals...);

			return log(LogPolicy::Log(Logger::Instance().out),vals...);
        };
        
        template<char C, class LogPolicy = Debug, typename ...Vals>
        static std::ostream& Log(Vals ...vals)
        {
			T::char_<C> TC;
			std::cout<<C<<std::endl;
			if constexpr (std::is_same<Debug,LogPolicy>::value)
				return log(LogPolicy::Log(Logger::Instance().file),vals...);

			return log(LogPolicy::Log(Logger::Instance().out),vals...);
        };
		
        template<typename Iterator,class LogPolicy = Debug>
        static void Log(Iterator begin, Iterator end, std::string name = "")
        {
			int j = 0;

			if(name != "")
				Logger::Log()<<name<<" :"<<std::endl;
			
			for(auto i = begin; i != end; ++i, ++j)
				Logger::Log<LogPolicy>()<<"\t"<<j<<" :"<<*i<<std::endl;
		};		
};

#ifndef NDEBUG
#   define LOG(message) Logger::Log()<<"File: "<<__FILE__ << " line " << __LINE__ << ": " << message << std::endl; 
#else
#   define LOG(message) 
#endif

template<typename T>
struct DebugDeleter 
{
  void operator()(T* t)  {	/*Logger::Log()<<"Pointer deleted."<<std::endl;*/ }
};

#endif
