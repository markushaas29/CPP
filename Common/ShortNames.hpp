#include "DateTimes.hpp"
#include "../CSV/Elements.hpp"
#include "../File/Raiffeisenbank.hpp"
#include "../File/TransferContainer.hpp"
#pragma once

namespace ShortNames
{
	using I = IBAN;
	using B = BIC;
	using N = Name;
	using DT = DateTimes::Date;
	using Q = Quantity<Sum>;
	using D = Bank::Direction;
	using E = Entry;
	
	using A = Bank::Raiffeisenbank<Configuration::Raiffeisenbank>;
	using TT = A::TransferType;
	using TP = std::tuple<I, B, N, DT, Q, D, E>;
	using TC = Bank::TransferContainer<TT>;
};

