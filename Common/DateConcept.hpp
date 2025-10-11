#include "../To/To.hpp"
#pragma once

template<typename T>
concept DateConcept = requires(T t)
{
	t.Prev();
	t.Next();
//	t.ToString();
//	t.Get();
//	t.Value();
};
