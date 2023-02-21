#include "DateTimes.hpp"
#include "../CSV/Elements.hpp"
#include "../CSV/KeyIndex.hpp"
#include "../File/Raiffeisenbank.hpp"
#include "../File/TransferContainer.hpp"
#include "../Home/AncilliaryRentalCosts/AncilliaryRentalCostItems.hpp"
#pragma once

namespace ShortNames
{
	using I = IBAN;
	using B = BIC;
	using N = Name;
	using DT = DateTimes::Date;
	using Y = DateTimes::Year;
	using M = DateTimes::Month;
	using DY = DateTimes::Day;
	using Q = Quantity<Sum>;
	using D = Bank::Direction;
	using E = Entry;
	
	using A = Bank::Raiffeisenbank<Configuration::Raiffeisenbank>;
	using TI = A::TransferItemContainerType;
	using TT = A::TransferType;
	using TP = std::tuple<I, B, N, DT, Q, D, E>;
	using ContData = std::shared_ptr<TT>;
	using TC = Bank::TransferContainer<ContData>;
	
	using KIC = CSV::KeyIndexContainer<A>;
	template<typename S> using BI = BuildingInsurance<S>;
	template<typename S> using WF = WasteFees<S>;
	template<typename S> using CS = ChimneySweeper<S>;
	template<typename S>using SW = Sewage<S>;
	template<typename S>using PT = PropertyTax<S>;
	template<typename S>using BC = BuildingCleaning<S>;
	template<typename S> using HT = Heating<S>;
	 
	template<typename S> using  ARItems = std::tuple<BuildingInsurance<S>,WasteFees<S>,ChimneySweeper<S>,Sewage<S>,PropertyTax<S>, BuildingCleaning<S>, Heating<S>>;
};

