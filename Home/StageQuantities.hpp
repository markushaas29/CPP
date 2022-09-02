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

class ApartmentArea: public CSVValue<ApartmentArea, Area, unsigned>
{
public:
	ApartmentArea(unsigned a): CSVValue(a) {};
	ApartmentArea(Quantity<Area> s): CSVValue(s) {};
	ApartmentArea& operator=(const ApartmentArea& a)
	{ 
		CSVValue<ApartmentArea, Area, unsigned>::operator=(a);
		return *this; 
	}
};

class IndividualUnit: public CSVValue<IndividualUnit, Scalar, unsigned>
{
public:
	IndividualUnit(unsigned a): CSVValue(a) {};
	IndividualUnit(Quantity<Scalar> s): CSVValue(s) {};
	IndividualUnit& operator=(const IndividualUnit& a)
	{
		CSVValue<IndividualUnit, Scalar, unsigned>::operator=(a);
		return *this; 
	}
};

class Rooms: public CSVValue<Rooms, Scalar, unsigned>
{
public:
	Rooms(unsigned r): CSVValue(r) {};
	Rooms(Quantity<Scalar> s): CSVValue(s) {};
	Rooms& operator=(const Rooms& a)
	{ 
		CSVValue<Rooms, Scalar, unsigned>::operator=(a);
		return *this; 
	}
};	

class Persons: public CSVValue<Persons, Scalar, unsigned>
{
public:
	Persons(const std::string& s = "0"): CSVValue(s) {};
	Persons(Quantity<Scalar> s): CSVValue(s) {};
	Persons& operator=(const Persons& a)
	{ 
		CSVValue<Persons, Scalar, unsigned>::operator=(a);
		return *this; 
	}
};	

class HeatingProportion: public CSVValue<HeatingProportion, Area, unsigned>
{
public:
	HeatingProportion(unsigned a): CSVValue(a) {};
	HeatingProportion(Quantity<Area> s): CSVValue(s) {};
	HeatingProportion& operator=(const HeatingProportion& a)
	{ 
		CSVValue<HeatingProportion, Area, unsigned>::operator=(a);
		return *this; 
	}
};	

class BuildingCleaningProportion: public CSVValue<BuildingCleaningProportion, Scalar, unsigned>
{
public:
	BuildingCleaningProportion(unsigned a): CSVValue(a) {};
	BuildingCleaningProportion(Quantity<Scalar> s): CSVValue(s) {};
	BuildingCleaningProportion& operator=(const BuildingCleaningProportion& a)
	{ 
		CSVValue<BuildingCleaningProportion, Scalar, unsigned>::operator=(a);
		return *this; 
	}
};	

class WaterCounter: public CSVValue<WaterCounter, Volume, double>
{
public:
	WaterCounter(std::string s = "0"): CSVValue(s) {};
};

class Advance: public CSVValue<Advance, Sum>
{
public:
	Advance(std::string s = "0"): CSVValue(s) {};
};

class IncidentalHeatingCosts: public CSVValue<IncidentalHeatingCosts, Sum>
{
public:
	IncidentalHeatingCosts(std::string s = "0"): CSVValue(s) {};
};	

class GarageRental: public CSVValue<GarageRental, Sum>
{
public:
	GarageRental(std::string s = "0"): CSVValue(s) {};
};	

class MonthlyRent: public CSVValue<MonthlyRent, Sum>
{
public:
	MonthlyRent(std::string s = "0"): CSVValue(s) {};
};	

template<> const char* CSVValue<IndividualUnit, Scalar, unsigned>::Key = "IndividualUnit";
template<> const char* CSVValue<ApartmentArea, Area, unsigned>::Key = "Area";
template<> const char* CSVValue<Rooms, Area>::Key = "Rooms";
template<> const char* CSVValue<Persons, Scalar, unsigned>::Key = "Persons";
template<> const char* CSVValue<Advance, Sum>::Key = "Advance";
template<> const char* CSVValue<IncidentalHeatingCosts, Sum>::Key = "IncidentalHeatingCosts";
template<> const char* CSVValue<MonthlyRent, Sum>::Key = "MonthlyRent";
template<> const char* CSVValue<GarageRental, Sum>::Key = "GarageRental";

#endif
