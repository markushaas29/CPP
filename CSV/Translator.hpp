#include <iostream>
#include <fstream>
#include <vector>
#include "../Common/Color.hpp"
#include "../Common/IClone.hpp"
#include "../String/String_.hpp"
#include "../File/Node.hpp"
#include "../ObjectFactory/Factory.hpp"

#pragma once

struct English		{   static constexpr const char* Identifier ="English"; };
struct German		{   static constexpr const char* Identifier ="German"; };
struct TokenId		{   static constexpr const char* Identifier ="TokenId"; };

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

class Translator
{   
		friend class Line;
		using Languages = std::tuple<English,German,TokenId>;
		template<typename> friend class Translate;

		template<typename L>
		class Translate: public ITranslate
		{   
			public:
				inline static const std::string Language = L::Identifier;
		        Translate(const std::string& s): word{s} {   }
		        virtual ~Translate(){  };
			private:
				std::unique_ptr<ITranslate> clone() const  { return std::make_unique<Translate>(word); };
				const std::string& get() const { return word; };
				std::string word;
		};
		
		class Line
		{   
				template<size_t N>
			    static auto build(auto&& res, const auto s)
			    {
			        if constexpr (std::tuple_size<Languages>()==N)
			            return std::move(res);
			        else
			        {
			            using Type = std::tuple_element_t<N,Languages>;
						res->push_back(Translator::factory[Type::Identifier](s[N]));
			            return build<N+1>(res,s);
			        }
			    }
				static auto read(const std::string& s)
				{
					auto split = String_::Split(s,';');
                   	auto result = build<0>(std::make_unique<std::vector<std::unique_ptr<ITranslate>>>(), split);
					return result;
				}
			public: 
		        Line(const std::string& s): translates{read(s)} {	}
		        virtual ~Line(){  };
				size_t Size() { return translates->size(); }
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

        const std::string fileName;
		int id = 1;
		std::unique_ptr<std::vector<std::unique_ptr<Line>>> translates;
		inline static Factory<ITranslate> factory;
		
		static auto read()
		{
			exec<0>(factory);
			auto lines = FS::ReadLines("/home/markus/Downloads/CSV_TestFiles_2/Translates.txt");
			auto result = std::make_unique<std::vector<std::unique_ptr<Line>>>();
			for(auto l : lines)
				result->push_back(std::make_unique<Line>(l));
			return result;
		}
    
		template<size_t N>
	    static void exec(Factory<ITranslate>& res)
	    {
	        if constexpr (std::tuple_size<Languages>()==N)
	            return;
	        else
	        {
	            using Type = std::tuple_element_t<N,Languages>;
				res.Register(Type::Identifier,[](const std::string& s) { return std::make_unique<Translate<Type>>(s); });
	            exec<N+1>(res);
	        }
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
            static Translator translator;
            return translator;
        };
};
