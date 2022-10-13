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
#include "CounterConfiguration.hpp"
#include <boost/mpl/for_each.hpp>
#include <boost/mpl/vector.hpp>
#include <map>
#include <chrono>
#include <ctime>
#include <memory>

#ifndef STAGEQUANTITIES_HPP
#define STAGEQUANTITIES_HPP

class ApartmentArea: public Value<ApartmentArea, Area, unsigned>
{
public:
	ApartmentArea(unsigned a): Value(a) {};
	ApartmentArea(Quantity<Area> s): Value(s) {};
	ApartmentArea& operator=(const ApartmentArea& a)
	{ 
		Value<ApartmentArea, Area, unsigned>::operator=(a);
		return *this; 
	}
};

class IndividualUnit: public Value<IndividualUnit, Scalar, unsigned>
{
public:
	IndividualUnit(unsigned a): Value(a) {};
	IndividualUnit(Quantity<Scalar> s): Value(s) {};
	IndividualUnit& operator=(const IndividualUnit& a)
	{
		Value<IndividualUnit, Scalar, unsigned>::operator=(a);
		return *this; 
	}
};

class Rooms: public Value<Rooms, Scalar, unsigned>
{
public:
	Rooms(unsigned r): Value(r) {};
	Rooms(Quantity<Scalar> s): Value(s) {};
	Rooms& operator=(const Rooms& a)
	{ 
		Value<Rooms, Scalar, unsigned>::operator=(a);
		return *this; 
	}
};	

class Persons: public Value<Persons, Scalar, unsigned>
{
public:
	Persons(const std::string& s = "0"): Value(s) {};
	Persons(Quantity<Scalar> s): Value(s) {};
	Persons& operator=(const Persons& a)
	{ 
		Value<Persons, Scalar, unsigned>::operator=(a);
		return *this; 
	}
};	

class HeatingProportion: public Value<HeatingProportion, Area, unsigned>
{
public:
	HeatingProportion(unsigned a): Value(a) {};
	HeatingProportion(Quantity<Area> s): Value(s) {};
	HeatingProportion& operator=(const HeatingProportion& a)
	{ 
		Value<HeatingProportion, Area, unsigned>::operator=(a);
		return *this; 
	}
};	

class BuildingCleaningProportion: public Value<BuildingCleaningProportion, Scalar, unsigned>
{
public:
	BuildingCleaningProportion(unsigned a): Value(a) {};
	BuildingCleaningProportion(Quantity<Scalar> s): Value(s) {};
	BuildingCleaningProportion& operator=(const BuildingCleaningProportion& a)
	{ 
		Value<BuildingCleaningProportion, Scalar, unsigned>::operator=(a);
		return *this; 
	}
};	

class WaterCounter: public Value<WaterCounter, Volume, double>
{
public:
	WaterCounter(std::string s = "0"): Value(s) {};
};

class Advance: public Value<Advance, Sum>
{
public:
	Advance(std::string s = "0"): Value(s) {};
};

class IncidentalHeatingCosts: public Value<IncidentalHeatingCosts, Sum>
{
public:
	IncidentalHeatingCosts(std::string s = "0"): Value(s) {};
};	

class GarageRental: public Value<GarageRental, Sum>
{
public:
	GarageRental(std::string s = "0"): Value(s) {};
};	

class MonthlyRent: public Value<MonthlyRent, Sum>
{
public:
	MonthlyRent(std::string s = "0"): Value(s) {};
};	

template<> const char* Value<IndividualUnit, Scalar, unsigned>::Key = "IndividualUnit";
template<> const char* Value<ApartmentArea, Area, unsigned>::Key = "Area";
template<> const char* Value<Rooms, Scalar, unsigned>::Key = "Rooms";
template<> const char* Value<BuildingCleaningProportion, Scalar, unsigned>::Key = "BuildingCleaningProportion";
template<> const char* Value<HeatingProportion, Area, unsigned>::Key = "HeatingProportion";
template<> const char* Value<Persons, Scalar, unsigned>::Key = "Persons";
template<> const char* Value<Advance, Sum>::Key = "Advance";
template<> const char* Value<IncidentalHeatingCosts, Sum>::Key = "IncidentalHeatingCosts";
template<> const char* Value<MonthlyRent, Sum>::Key = "MonthlyRent";
template<> const char* Value<GarageRental, Sum>::Key = "GarageRental";

#endif
