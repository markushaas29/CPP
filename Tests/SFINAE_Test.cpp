#include<iostream>

template<bool, typename T = void>
struct EnableIfT{}

template<true, typename T>
struct EnableIfT<true,T>{
	using Type = T;	
}

template<bool Cond, typename T = void>
struct EnableIf = typename EnableIfT<Cond, T>::Type

template<typename T>
EnableIf<true , T> FT(T t){std::cout<<"EnableIf<true , T>"<<std::endl; };

template<>
EnableIf<true , T> FT<int>(int t){std::cout<<"EnableIf<true , T>"<<std::endl;};

int main()
{
	return 0;
}
