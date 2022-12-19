#pragma once

template<typename Derived>
struct ConfigurationBase
{
	Type = Derived
};

template<typename T>
struct Configuration: ConfigurationBase<Configuration<T>>
{
	
};
