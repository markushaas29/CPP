#include "../String/String_.hpp"
#include "../Logger/Logger.hpp"
#include "../Common/DateTimes.hpp"
#include "../CSV/CSV.hpp"
#include "../File/Info.hpp"
#include "../File/Node.hpp"
#include "../Unit/SIPrefix.hpp"
#include "../Home/Counter.hpp"
#include "../Wrapper/Wrapper.hpp"
#include <map>
#include <memory>

#ifndef INPUTMANAGER_HPP
#define INPUTMANAGER_HPP

template<typename ConfigT>
class InputManager
{
public:
	using Config = ConfigT;
	using InputManagerType = InputManager<Config>;
	using ReadingContainerType = std::vector<std::string>;
	using OutIterator = ReadingContainerType::const_iterator;
	using InputIterator = std::vector<std::string>::const_iterator;
	
	inline static constexpr const char* Filename = "Input";

	static InputManager& Instance()
	{
		static InputManager instance;
		return instance;
	}
	
	template<typename Separator = T::char_<'\t'>>
	static std::ostream& Display(std::ostream& out)
	{
		//~ for(auto it = readings->cbegin(); it != readings->cend(); ++it)
			//~ (*it)->Display(out);
		
		return out;
	}
	
	static void Parse(InputIterator begin, InputIterator end)
	{
		Instance();
		for(auto it = begin; it != end; ++it)
		{
			Logger::Log()<<"InputManagerValue: "<<*it<<"\t"<<CE1::GetName()<<std::endl;
		}
	}
	
	static void Write(const std::string sourcePath = ".")
	{
		//~ Logger::Log<Info>()<<"Write InputManager: "<<GetName()<<" to: "<<csv->GetDestinationPath()<<std::endl;
		//~ csv->Write<InputManagerType>();
	}
	
	template<typename Cont>
	static void RegisterTo(Cont& cont)
	{
		cont.insert(std::make_pair(Filename,  &InputManagerType::Parse));
		Logger::Log()<<cont.size()<<std::endl;
	}
	
	template<typename Op>
	decltype(auto) Get(const Op&& op){ return op(readings->cbegin(),readings->cend());	}
		
	static OutIterator ReadingsBegin() { return readings->cbegin(); }
	static OutIterator ReadingsEnd() { return readings->cend(); }		
private:
	inline static std::unique_ptr<ReadingContainerType> readings = std::make_unique<ReadingContainerType>();
	
	//~ inline static std::unique_ptr<FS::FileInfo> fileInfo = std::unique_ptr<FS::FileInfo>(new FS::FileInfo(std::filesystem::path(std::string(DestinationPath) + std::string(Name) )));
	//~ inline static std::unique_ptr<FS::CSV> csv = std::unique_ptr<FS::CSV>(new FS::CSV(fileInfo.get()));
	
	InputManager()	{ 	Logger::Log<Info>()<<"InputManager initialized "<<std::endl; 	};
	~InputManager()	{ /*Logger::Log()<<"Destructor"<<std::endl;*/ }
	InputManager& operator=(const InputManager&) = delete;
	InputManager(const InputManager& c) = delete;
};

template<typename C, typename S = T::char_<'\t'>>
std::ostream& operator<<(std::ostream& strm, const InputManager<C> c){	return c.Display(strm);}

#endif
