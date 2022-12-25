
#pragma once

namespace DateTimes { struct Date; }

template<typename Account, typename TupleT>	class Transfer;
	
template<typename ItemT, typename Account, typename TupleT>	const ItemT& GetTransfer(Transfer<Account,TupleT>const& t);

struct TransferComparer
{
	struct Date
	{
		template<typename T>
		bool operator()(const T& l, const T& r) {return GetTransfer<DateTimes::Date>(*r) == GetTransfer<DateTimes::Date>(*l);};
	};
};
