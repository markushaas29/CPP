#pragma once

template<typename Derived>
struct ConfigurationBase
{
	using Type = Derived;
};

template<class T>
struct Configuration: ConfigurationBase<Configuration<T>>
{
	
};

template<typename S> class AncilliaryRentalCostsContainer;
template<typename ConfigT> class Stage;

template<typename T>
struct Configuration<AncilliaryRentalCostsContainer<T>>: ConfigurationBase<Configuration<AncilliaryRentalCostsContainer<T>>>
{
	using Stage = T;
};
