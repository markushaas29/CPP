#include <iostream>
#include <boost/mpl/vector.hpp>
#include "Counter.hpp"
#include "CounterContainer.hpp"
#include "Stage.hpp"
#include "StageContainer.hpp"
#include "House.hpp"
#include "../Tuple/Tuple.hpp"
#include "../Unit/Unit.hpp"
#include "../Logger/Logger.hpp"

int main()
{       
    std::cout<<"START"<<std::endl;
	
	House<HouseConfiguration>::Instance().Display(std::cout);
	House<HouseConfiguration>::Instance().Calculate();
	
    std::cout<<"END"<<std::endl;

    return 0;
}
