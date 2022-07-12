#include "../String/String_.hpp"
#include "../Logger/Logger.hpp"
#include "../Unit/Unit.h"
#include "../Quantity/Quantity.h"
#include "../CSV/CSV.hpp"
#include "../File/Info.hpp"
#include "../File/Node.hpp"
#include "../Unit/SIPrefix.hpp"
#include "../Wrapper/Wrapper.hpp"
#include "Parser.hpp"
#include "CounterContainer.hpp"
#include "StageQuantities.hpp"
#include <boost/mpl/for_each.hpp>
#include <boost/mpl/vector.hpp>
#include <map>
#include <chrono>
#include <ctime>
#include <memory>

#ifndef STAGECONFIGURATION_HPP
#define STAGECONFIGURATION_HPP

using StageMap = std::map<std::string, std::string>;

struct TOP{ inline  static constexpr const char* Name = "Top"; };
struct MIDDLE{ inline  static constexpr const char* Name = "Middle"; };
struct BOTTOM{ inline  static constexpr const char* Name = "Bottom"; };

template<typename Derived, typename TColdWaterCounter, typename THotWaterCounter, typename TEnergyCounter,int N, unsigned A, unsigned R, typename T = TOP, unsigned I = 1>
struct StageConfiguration
{
	using Type = Derived;
	using ColdWaterCounter = TColdWaterCounter;
	using HotWaterCounter = THotWaterCounter;
	using EnergyCounter = TEnergyCounter;
	static constexpr int Units = I;
	static constexpr int Number = N;
	static constexpr unsigned Area = A;
	static constexpr unsigned Rooms = R;
	static constexpr const char* Name = T::Name;
};

struct TopConfiguration:StageConfiguration<TopConfiguration, CTCW, CTHW, CVat,2,58,5,TOP> { };
struct MiddleConfiguration:StageConfiguration<MiddleConfiguration,CMCW, CMHW, CE1,1,101,7,MIDDLE> { };
struct BottomConfiguration:StageConfiguration<BottomConfiguration,CBCW, CBHW, CE1,0,101,7,BOTTOM> { };

#endif
