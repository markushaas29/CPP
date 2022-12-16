#include <sstream>

#pragma once

namespace
{
	
template<typename Target=std::string, typename Source=std::string>
Target To(Source arg)
{
	std::stringstream buf;
	Target result;
	
	if(!(buf << arg) || !(buf >> result) || !(buf >> std::ws).eof())
		throw std::runtime_error("to<>() failed");
	return result;
}

}
