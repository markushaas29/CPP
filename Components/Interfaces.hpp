#include "../String/String_.hpp"
#include "../Logger/Logger.hpp"
#include "../Common/DateTimes.hpp"
#include "../CSV/Elements.hpp"
#include "../Wrapper/Wrapper.hpp"
#include <map>
#include <memory>

#pragma once

class IIn
{
public:
	virtual std::istream& operator()(std::istream& i) = 0;
	virtual ~IIn(){};
};

class IOut
{
public:
	virtual std::ostream& operator()(std::ostream& i) = 0;
	virtual std::unique_ptr<std::ofstream> operator()(std::unique_ptr<std::ofstream> of) = 0;
	virtual ~IOut(){};
};

class IIO: public IIn, public IOut
{
public:
	virtual ~IIO(){};
};

class IParse
{
public:
	using Iterator = std::vector<std::string>::const_iterator;
	virtual void operator()(Iterator begin, Iterator end) = 0;
	virtual ~IParse(){};
};
