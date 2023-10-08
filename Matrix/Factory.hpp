#pragma once

template<typename,typename> class IMatrixQuery;
template<typename> class IMatrixCategory;

template<typename T>//, template<typename> class Q = IMatrixQuery, template<typename> class C = IMatrixCategory>
//std::unique_ptr<IMatrixQuery<T, typename T::ElementType>> Create(const T& t, const std::string& q, const std::string& c)
void Create(const T& t, const std::string& q, const std::string& c)
{
	switch(c)
	{
		case EquivalenceCat<std::string>::TypeId:
			std::cout<<"EQ";
			return;
		case ContainCat<std::string>::TypeId:
			std::cout<<"EQ";
			return;
	}
}
