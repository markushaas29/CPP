#include <sstream>

#pragma once

namespace String_
{
	
template<typename Target=std::string, typename Source=std::string>
Target ParseTo(Source arg)
{
	std::stringstream buf;
	Target result;
	
	if(!(buf << arg) || !(buf >> result) || !(buf >> std::ws).eof())
		throw std::runtime_error("to<>() failed");
	return result;
}

}
