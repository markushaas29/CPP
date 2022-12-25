#pragma omce

struct ReadingComparer
{
	struct Date
	{
		template<typename T>
		bool operator()(const T& l, const T& r) {return l->Date > r->Date;};
	};
};
