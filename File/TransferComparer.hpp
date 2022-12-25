#pragma once

struct TransferComparer
{
	struct Date
	{
		template<typename T>
		bool operator()(const T& l, const T& r) {return true;};
	};
};
