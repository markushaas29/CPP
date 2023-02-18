#include "DateTimes.hpp"
#include "../CSV/Elements.hpp"
#include "../CSV/KeyIndex.hpp"
#include "../File/Raiffeisenbank.hpp"
#include "../File/TransferContainer.hpp"
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
};

