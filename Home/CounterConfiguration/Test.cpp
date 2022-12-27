#include <iostream>
#include "../CounterConfiguration.hpp"
#include "../Counter.hpp"

int main()
{
	GasConfiguration::Display(std::cout);
	VattenfallEnergyConfiguration::Display(std::cout);

	using c = Counter<GasConfiguration>;
	c::Display(std::cout);

	std::cout<<"Test"<<std::endl;
	
	return 0;
}
