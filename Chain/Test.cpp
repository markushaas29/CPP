#include <iostream>
#include "IChain.hpp"

class IChainTest
{
    public:
        int Run()
{       
    std::cout<<"START Chain"<<std::endl;
	auto ic = std::make_unique<IChain>();
	std::cout<<"END"<<std::endl;

    return 0;
}
};
