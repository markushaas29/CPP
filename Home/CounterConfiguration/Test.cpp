#include <iostream>
#include "../CounterConfiguration.hpp"

int main()
{
	GasConfiguration::Display(std::cout);
	VattenfallEnergyConfiguration::Display(std::cout);

	//~ std::cout<<GasConfiguration<<std::endl;

	std::cout<<"Test"<<std::endl;
	
	return 0;
}
