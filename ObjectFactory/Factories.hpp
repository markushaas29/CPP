#include <map>
#include <functional>
#include <memory>
#include <string_view>
#include "Factory.hpp"
#include "../Is/Is.hpp"
#include "../CSV/Elements.hpp"
#include "../String/Format.hpp"

#pragma once

//Factory<IElement> ElementFactory;
//ElementFactory.Register("I",[](const std::string& s) { return std::make_unique<IBAN>(std::string(s)); });
//ElementFactory.Register("B",[](std::string_view s) { return std::make_unique<BIC>(s); });
