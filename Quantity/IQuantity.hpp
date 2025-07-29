#include <string>
#include <iostream>
#include <cmath>
#include <sstream>
#include <regex>
#include "QuantityRatio.hpp"
#include "../Unit/Unit.hpp"
#include "../Unit/UnitRatio.hpp"
#include "../Logger/Logger.hpp"
#include "../String/StringParser.hpp"
#include "../Common/Make/Make.hpp"
#include "../String/String_.hpp"
#include "../To/To.hpp"

#pragma once

template<typename T1 = double>
class IQuantity
{
public:
	using ValueType = T1;

    virtual const std::string UnitSign() const = 0;
	virtual const std::string SiUnit() const = 0;
    
};
